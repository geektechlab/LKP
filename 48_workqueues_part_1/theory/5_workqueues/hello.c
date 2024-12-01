#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

/* workqueue doesn't guarantee that it will get scheduled on same processor */

MODULE_LICENSE("GPL");

struct work_struct work;

static void work_fn(struct work_struct *work)
{
	pr_info("processor id:%d\tdeferred work execution\n", smp_processor_id());
}

static int test_tasklet_init(void)
{
    pr_info("processor id:%d: In init\n", smp_processor_id());
	INIT_WORK(&work, work_fn);
	queue_work(system_wq, &work);
	return 0;
}

static void test_tasklet_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

module_init(test_tasklet_init);
module_exit(test_tasklet_exit);
