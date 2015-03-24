// dev_t 类型包含在linux/types.h中
#include <linux/types.h>

// 获取主、次设备号MAJOR()和MINOR()宏定义在linux/kdev_t.h
#include <linux/kdev_t.h>

// 申请设备号register_chrdev_region()、alloc_chrdev_region()
// 释放设备号unregister_chrdev_region()等，定义在linux/fs.h中
// file_operations结构用于建立驱动程序到设备号的连接
#include <linux/fs.h>

// struct cdev
#include <linux/cdev.h>

// THIS_MODULE
#include <linux/module.h>

// unsigned long copy_to_user(void __user *to, const void *from, unsigned long count);
// unsigned long copy_from_user(void *to, const void __user *from, unsigned long count);
#include <linux/uaccess.h>
/****************************************************************************************************/
/****************************************************************************************************/
/*                           header end                                                             */
/****************************************************************************************************/
/****************************************************************************************************/

struct scull_dev {
    struct scull_qset *data;     // 指向第一个量子集的指针
    int quantum;                 // 当前量子的大小
    int qset;                    // 当前数组的大小
    unsigned long size;          // 保存在其中的数据总量
    unsigned int access_key;     // 由sculluid和scullpriv调用
    struct semaphore sem;        // 互斥信号量
    struct cdev cdev;            // 字符设备结构
};
struct scull_qset {
    void **data;
    struct scull_qset *next;
};
static unsigned int count = 1;
static struct scull_dev *sdev;
static dev_t scull_major;
static dev_t scull_minor;
static dev_t dev;
static void scull_setup_cdev(struct scull_dev *dev, int index);
struct scull_qset *scull_follow(struct scull_dev *dev, int index);
int scull_trim(struct scull_dev *dev);
int scull_open(struct inode *inode, struct file *filp);
int scull_release(struct inode *inode, struct file *filp);
ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

struct file_operations scull_fops = {
    .owner = THIS_MODULE,
    // .llseek = scull_llseek,
    .read = scull_read,
    .write = scull_write,
    // .ioctl = scull_ioctl,
    .open = scull_open,
    .release = scull_release,
};


// static void scull_setup_cdev(struct scull_dev *dev, int index);
// int scull_open(struct inode *inode, struct file *filp);
// int scull_release(struct inode *inode, struct file *filp);
// int scull_trim(struct scull_dev *dev);
// ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
// ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
/*
**  iovec定义在<linux/uio.h>
**  struct iovec{
**      void __user *iov_base;
**      __kernel_size_t iov_len;
**  }
**  ssize_t (*readv)(struct file *filp, const struct iovec *iov, unsigned long count, loff_t *ppos);
**  ssize_t (*writev)(struct file *filp, const struct iovec *iov, unsigned long count, loff_t *ppos);
*/
// unsigned long copy_to_user(void __user *to, const void *from, unsigned long count);
// unsigned long copy_from_user(void *to, const void __user *from, unsigned long count);
/*
**    不检查参数：
**  __copy_to_user
**  __copy_from_user
*/
