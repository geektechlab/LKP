#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");

static int my_value1 = 0;
static int my_value2 = 0;
static struct kobject *my_kobj;

static ssize_t my_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
	pr_info("attribute name:%s\n", attr->attr.name);
	if (strcmp(attr->attr.name, "my_file1") == 0)
		return sprintf(buf, "%d\n", my_value1);
	else
		return sprintf(buf, "%d\n", my_value2);
}

static ssize_t my_store(struct  kobject *kobj, struct kobj_attribute *attr,
                        const char *buf,size_t count)
{
	int ret, tmp;

	pr_info("attribute name:%s\n", attr->attr.name);
	ret = kstrtoint(buf, 10, &tmp);
        if (ret < 0)
                return ret;

	if (strcmp(attr->attr.name, "my_file1") == 0)
		my_value1 = tmp;
	else
		my_value2 = tmp;
        return count;
}

static struct kobj_attribute my_attr1 = __ATTR(my_file1, 0644, my_show, my_store);
static struct kobj_attribute my_attr2 = __ATTR(my_file2, 0644, my_show, my_store);

static int __init test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);

	my_kobj = kobject_create_and_add("my_kobj", NULL);
        if (!my_kobj)
                return -ENOMEM;

	if (sysfs_create_file(my_kobj, &my_attr1.attr)) {
		pr_err("unable to create sysfs file my_file1\n");
		return  -1;
	}
	pr_info("sysfs file:my_file1 created successfully\n");
	if (sysfs_create_file(my_kobj, &my_attr2.attr)) {
		pr_err("unable to create sysfs file my_file2\n");
		return  -1;
	}
	pr_info("sysfs file:my_file1 created successfully\n");
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	kobject_put(my_kobj);
}

module_init(test_hello_init);
module_exit(test_hello_exit);