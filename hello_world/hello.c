#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
static char hello_in[1024] = {"Hello, World."};
static char hello_ex[1024] = {"Goodbye, CJ"};
static int hello_init(void)
{
    printk(KERN_DEBUG "%s\n", hello_in);
    return 0;
}
static void hello_exit(void)
{
    printk(KERN_DEBUG "%s\n", hello_ex);
}
MODULE_LICENSE("Dual BSD/GPL");
module_init(hello_init);
module_exit(hello_exit);
