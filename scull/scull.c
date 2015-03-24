#include "scull.h"
#include <linux/init.h>
MODULE_LICENSE("Dual BSD/GPL");
static int scull_init(void)
{
    int result;
    printk(KERN_DEBUG "开始获取设备号");
    // 检查是否手动指定设备号
    if (scull_major)
    {
        dev = MKDEV(scull_major, scull_minor);
        result = register_chrdev_region(dev, count, "scull");
    } else {
        result = alloc_chrdev_region(&dev, scull_minor, count, "scull");
        scull_major = MAJOR(dev);
    }
    if (result < 0)
        printk(KERN_DEBUG "设备号获取失败");
    scull_setup_cdev(sdev, 0);
    printk(KERN_DEBUG "设备号获取成功%d", MKDEV(scull_major, scull_minor));
    return 0;
}
static void scull_setup_cdev(struct scull_dev *dev, int index)
{
    int err, devno = MKDEV(scull_major, scull_minor + index);
    cdev_init(&(dev->cdev), &scull_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &scull_fops;
    err = cdev_add(&(dev->cdev), devno, count);
    if (err)
        printk(KERN_DEBUG "Error %d adding scull %d", err, index);
    printk(KERN_DEBUG "setup done");
}
int scull_trim(struct scull_dev *dev)
{
    struct scull_qset *next, *dptr;
    int qset = dev->qset;
    int i;
    printk(KERN_DEBUG "Running scull_trim");
    for (dptr = dev->data; dptr; dptr = next)
    {
        if (dptr->data)
        {
            for (i = 0; i < qset; i++)
                kfree(dptr->data[i]);
            kfree(dptr->data);
            dptr->data = NULL;
        }
        next = dptr->next;
        kfree(dptr);
    }
    dev->size = 0;
    dev->quantum = 0;
    dev->qset = 0;
    dev->data = NULL;
    return 0;
}
int scull_open(struct inode *inode, struct file *filp)
{
    /*
    **  open应完成如下工作：
        1) 检查设备特定的错误（诸如设备未就绪或类似的硬件问题）
        2）如果设备是首次打开，则对其进行初始化
        3）如果有必要，更新f_op指针。
        4）分配并填写置于filp->private_data里的数据结构
    */
    struct scull_dev *dev;
    printk(KERN_DEBUG "Openning scull_open");
    // dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    filp->private_data = sdev;
    if ((filp->f_flags & O_ACCMODE) == O_WRONLY)
        scull_trim(dev);
    return 0;
}
int scull_release(struct inode *inode, struct file *filp)
{
    printk(KERN_DEBUG "Running scull_release");
    return 0;
}
ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    /*
    **      1）如果返回值等于传递给 read系统调用的count参数，则说明所请求的字节数传输成功完成。
    **      2) 如果返回值是正的，但是比count小，则说明只有部分数据成功传送。这种情况因设备的不同
    **     可能有很多原因。大部分情况下，程序会重新读数据。例如，如果用fread函数读数据，这个库函
    **     数就会不断调用系统调用，直至请求的数据传输完毕为止。
    **      3）如果返回值为0,则表示已经到达了文件尾。
    **      4) 负值意味着发生了错误，该值指明了发生什么错误，错误码在<linux/errno.h>中定义。比如
    **     这样的一些错误： -EINTR(系统调用被中断)或-EFAULT(无效地址)
    */
    struct scull_dev *dev = filp->private_data;
    struct scull_qset *dptr;  // 第一个链表项
    int quantum = dev->quantum, qset = dev->qset;
    int itemsize = quantum * qset; // 该链表项中有多少字节
    int item, s_pos, q_pos, rest;
    ssize_t retval = 0;
    printk(KERN_DEBUG "Running scull_read");
    if (down_interruptible(&(dev->sem)))
        return -ERESTARTSYS;
    if (*f_pos >= dev->size)
        goto out;
    if (*f_pos + count > dev->size)
        count = dev->size - *f_pos;

    // 在量子集中寻找链表项， qset索引以及偏移量
    item = (long)*f_pos / itemsize;
    rest = (long)*f_pos % itemsize;
    s_pos = rest / quantum; q_pos = rest % quantum;

    // 沿该链表前行， 直到正确的位置
    dptr = scull_follow(dev, item);

    if (dptr == NULL || !dptr->data || !dptr->data[s_pos])
        goto out;

    // 读取该量子的数据知道结尾
    if (count > quantum - q_pos)
        count = quantum - q_pos;
    if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count))
    {
        retval = -EFAULT;
        goto out;
    }
    *f_pos += count;
    retval = count;
out:
    up(&dev->sem);
    return retval;
}
struct scull_qset *scull_follow(struct scull_dev *dev, int index)
{
    struct scull_qset *data = dev->data;
    printk(KERN_DEBUG "Running scull_sset");
    while (index--)
    {
        if (data->next == NULL)
            return data;
        data = data->next;
    }
    return NULL;
}

ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    /*
    **      1）如果返回值等于count,则完成了所请求数目的字节传送
    **      2) 如果返回值是正的，但小于count，则只传输了部分数据。程序很可能在此试图写下余下的数据。
    **      3）如果值为0,意味着什么也没写入。这个结果不是错误，而且也没有理由返回一个错误码。
    **      4) 负值意味着发生了错误，与read相同，有效的错误码定义在<linux/errno.h>中
    */
    struct scull_dev *dev = filp->private_data;
    struct scull_qset *dptr;
    int quantum = dev->quantum, qset = dev->qset;
    int itemsize = quantum * qset;
    int item, s_pos, q_pos, rest;
    ssize_t retval = -ENOMEM; // goto out 使用

    printk(KERN_DEBUG "Running scull_write");
    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    // 在量子集中寻找链表项，qset索引以及偏移量
    item = (long)*f_pos / itemsize;
    rest = (long)*f_pos % itemsize;
    s_pos = rest / quantum;
    q_pos = rest % quantum;

    // 沿链表前行，直到正确的位置
    dptr = scull_follow(dev, item);
    if (dptr == NULL)
        goto out;
    if (!dptr->data)
    {
        dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
        if (!dptr->data)
            goto out;
        memset(dptr->data, 0, qset * sizeof(char *));
    }
    if (!dptr->data[s_pos])
    {
        dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
        if (!dptr->data[s_pos])
            goto out;
    }
    // 将数据写入该量子，直到结尾
    if (count > quantum - q_pos)
        count = quantum - q_pos;
    if (copy_from_user(dptr->data[s_pos] + q_pos, buf, count))
    {
        retval = -EFAULT;
        goto out;
    }
    *f_pos += count;
    retval = count;
    if (dev->size < *f_pos)
        dev->size = *f_pos;
out:
    up(&dev->sem);
    return retval;
}
void scull_cleanup_module(void)
{
    dev_t dev = MKDEV(scull_major, scull_minor);//获得设备号
    cdev_del(&sdev->cdev);
    unregister_chrdev_region(dev, 0);//释放设备编号，第一个设备号，第二个设备数。
    printk(KERN_DEBUG "Running scull_cleanup_module");
}
module_init(scull_init);
module_exit(scull_cleanup_module);
