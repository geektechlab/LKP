#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>

/* my_kobj folder gets created in sys ( sysfs ). Just do ls /sys */

MODULE_LICENSE("GPL");
static struct kobject *my_kobj;

static int __init test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);
	my_kobj = kobject_create_and_add("my_kobj", NULL);
	if (!my_kobj)
		return -ENOMEM;
	
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	kobject_put(my_kobj);
}

module_init(test_hello_init);
module_exit(test_hello_exit);