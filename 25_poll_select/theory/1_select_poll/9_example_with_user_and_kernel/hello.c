#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/wait.h>

int base_minor = 0;
int count = 1;
int write_count = 0;
dev_t devicenumber;

static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev *mycdev = NULL;
int flag = 0;
wait_queue_head_t event_queue;
MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
        return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer,
		size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	wait_event_interruptible(event_queue, flag);
	flag = 0;
	return 0;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer,
		size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	flag = 1;
	wake_up_interruptible(&event_queue);
	return count;
}

struct file_operations device_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static int test_hello_init(void)
{
	init_waitqueue_head(&event_queue);
	class = class_create(THIS_MODULE, "myclass");

	if (!alloc_chrdev_region(&devicenumber, base_minor, count, "mydevice")) {
		printk("Device number registered\n");
		printk("Major number received:%d\n", MAJOR(devicenumber));

		device = device_create(class, NULL, devicenumber, NULL, "mydevice");
		mycdev = cdev_alloc();
		if (mycdev) {
			mycdev->ops = &device_fops;
			mycdev->owner = THIS_MODULE;
			cdev_add(mycdev, devicenumber, count);
		}

	}
	else
		printk("Device number registration Failed\n");

	return 0;
}

static void test_hello_exit(void)
{
	device_destroy(class, devicenumber);
	class_destroy(class);
	cdev_del(mycdev);
	unregister_chrdev_region(devicenumber, count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
