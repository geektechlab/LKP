#include <linux/module.h>
#include <linux/kallsyms.h>

/* here, we disable write protected bit and then update system call table to point to my_open function */
/* while returning, we set back to original */
/* this works */

MODULE_LICENSE("GPL");
char *sym_name = "sys_call_table";

typedef asmlinkage long (*sys_call_ptr_t)(const struct pt_regs *);
static sys_call_ptr_t *sys_call_table;

sys_call_ptr_t old_open;

static asmlinkage long my_open(const struct pt_regs *regs)
{
	pr_info("%s\n",__func__);
	return old_open(regs);
}

static int __init hello_init(void)
{
	sys_call_table = (sys_call_ptr_t *)kallsyms_lookup_name(sym_name);
	old_open = sys_call_table[__NR_open];
	// Temporarily disable write protection
	write_cr0(read_cr0() & (~0x10000));
	sys_call_table[__NR_open] = my_open;
	// Re-enable write protection
	write_cr0(read_cr0() | 0x10000);

	return 0;
}

static void __exit hello_exit(void)
{
	// Temporarily disable write protection
	write_cr0(read_cr0() & (~0x10000));
	sys_call_table[__NR_open] = old_open;
	// Re-enable write protection
	write_cr0(read_cr0() | 0x10000);
}

module_init(hello_init);
module_exit(hello_exit);
