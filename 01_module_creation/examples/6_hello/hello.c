/* see what happens when we return -1 from test_hello_init(), so we will not be able to load in kernel space, but it wil print message */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
static int test_hello_init(void)
{
    printk(KERN_INFO"%s: In init\n", __func__);
    return -1;
}

static void test_hello_exit(void)
{
    printk(KERN_INFO"%s: In exit\n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
