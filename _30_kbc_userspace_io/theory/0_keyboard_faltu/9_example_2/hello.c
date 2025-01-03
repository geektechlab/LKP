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
	//0xAD (Disable keyboard interface) - Sets bit 4 of command byte and disables all communication with keyboard

	i8042_write_command(0xAD);

	ssleep(5);

	//0xAE (Enable keyboard interface) - Clears bit 4 of command byte and re-enables communication with keyboard
	i8042_write_command(0xAE);

	return 0;
}

void  __exit hello_exit(void)
{

}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
