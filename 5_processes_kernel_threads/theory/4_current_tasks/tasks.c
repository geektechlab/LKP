#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <asm/current.h>

char buffer[256];
char * get_task_state(long state)
{
    switch (state) {
        case TASK_RUNNING:
            return "TASK_RUNNING";
        case TASK_INTERRUPTIBLE:
            return "TASK_INTERRUPTIBLE";
        case TASK_UNINTERRUPTIBLE:
            return "TASK_UNINTERRUPTIBLE";
        case __TASK_STOPPED:
            return "__TASK_STOPPED";
        case __TASK_TRACED:
            return "__TASK_TRACED";
        default:
        {
            sprintf(buffer, "Unknown Type:%ld\n", state);
            return buffer;
        }
    }
}

static int test_tasks_init(void)
{
    struct task_struct *task;
    unsigned int process_count = 0;
    pr_info("%s: In init\n", __func__);
    /*define for_each_process(p) \
     for (p = &init_task ; (p = next_task(p)) != &init_task ; )
     */

    /* it will print only few processes, because we are not running till entire doubly linked list */
    /* we are running only till init_task */
    for(task = current ; task != &init_task ; task = task->parent){
        pr_info("Process: %s\t PID:[%d]\t State:%s\n", 
                task->comm, task->pid,
                get_task_state(task->state));
        process_count++;    
    }
    pr_info("Number of processes:%u\n", process_count);
    return 0;
}

static void test_tasks_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

MODULE_LICENSE("GPL");
module_init(test_tasks_init);
module_exit(test_tasks_exit);
