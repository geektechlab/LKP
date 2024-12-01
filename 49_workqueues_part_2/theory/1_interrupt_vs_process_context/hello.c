#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/sched.h>

/* workqueue runs in process context. tasklet and softirq runs interrupt context */

MODULE_LICENSE("GPL");

struct work_struct work;
int cpu = 2;
module_param(cpu, int, 0);

typedef struct my_work{
        struct work_struct work;
        char data[20];
}my_work;

my_work deferred_work;

void print_context(void)
{
        if (in_interrupt()) {
                pr_info("Code is running in interrupt context\n");
        } else {
                pr_info("Code is running in process context\n");
        }
}

static void work_fn(struct work_struct *work)
{
	my_work *defer_work = (my_work *)container_of(work, my_work, work);
	pr_info("processor id:%d\tdeferred work execution\n", smp_processor_id());
	pr_info("Data:%s\n", defer_work->data);
	pr_info("current pid : %d , current process : %s\n",current->pid, current->comm);

	print_context();
}

static int test_tasklet_init(void)
{
    pr_info("processor id:%d: In init\n", smp_processor_id());
	print_context();

	INIT_WORK(&deferred_work.work, work_fn);
	strcpy(deferred_work.data, "Linux is easy");

	if (schedule_work_on(cpu, &deferred_work.work))
		pr_info("work queued\n");
	else
		pr_err("work queuing failed\n");

	return 0;
}

static void test_tasklet_exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test_tasklet_init);
module_exit(test_tasklet_exit);
