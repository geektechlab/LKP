#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>

/* run [ dmesg -hW ] as daemon to print logs while inserting this module */
/* then insert module and then run [ ls /dev/mychardev -al ] to print entry. Also verify entry removal after removing module */

int base_minor = 0;
char *device_name = "mychardev";
int count = 1;
dev_t devicenumber;

static struct class *class;
static struct device *device;

MODULE_LICENSE("GPL");
static int test_hello_init(void)
{
	/* creating a class */
	class = class_create(THIS_MODULE, "myclass");

	/* dynamically create major number */
	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name))
	{
		printk("Device number registered\n");
		printk("Major number received:%d\n", MAJOR(devicenumber));
		/* create node */
		device = device_create(class, NULL, devicenumber, NULL, device_name);
	}
	else
		printk("Device number registration Failed\n");

	return 0;
}

static void test_hello_exit(void)
{
	unregister_chrdev_region(devicenumber, count);
	device_destroy(class, devicenumber);
	class_destroy(class);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
