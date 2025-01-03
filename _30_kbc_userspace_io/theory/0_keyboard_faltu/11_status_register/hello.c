#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#define I8042_COMMAND_REG       0x64
#define I8042_STATUS_REG        0x64
#define I8042_DATA_REG          0x60

static inline int i8042_read_data(void)
{
        return inb(I8042_DATA_REG);
}

static inline int i8042_read_status(void)
{
        return inb(I8042_STATUS_REG);
}

static inline void i8042_write_data(int val)
{
        outb(val, I8042_DATA_REG);
}

static inline void i8042_write_command(int val)
{
        outb(val, I8042_COMMAND_REG);
}

int __init  hello_init(void)
{
	
	int status = i8042_read_status();

	if (status & 0x01)
		pr_info("Output buffer status full\n");
	else
		pr_info("Output buffer status empty\n");

	if (status & 0x02)
		pr_info("Input buffer status full\n");
	else
		pr_info("Input buffer status empty\n");

	if (status & 0x08)
		pr_info("Command byte\n");
	else
		pr_info("Data Byte\n");

	if (status & 0x10)
		pr_info("Keyboard enabled\n");
	else
		pr_info("Keyboard disabled\n");

	return 0;
}

void  __exit hello_exit(void)
{

}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
