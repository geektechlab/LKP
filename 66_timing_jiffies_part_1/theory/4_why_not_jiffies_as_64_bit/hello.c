#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/delay.h>

static int __init test_hello_init(void)
{
	u64 jiffie1, jiffie2;

	pr_info("%s\n", __func__);
	jiffie1 = jiffies;
	jiffie2 = get_jiffies_64();
	pr_info("%s: Jiffie1:%llu\n", __func__, jiffie1);
	pr_info("%s: Jiffie2:%llu\n", __func__, jiffie2);

	return -1;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);