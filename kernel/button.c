/*
Chris Feldman & Emily Lam
Project brACE by Team ACE
EC535 -- Professor Coskun

button.c
*/

// Necessary includes for device drivers
#include <asm/system.h>      /* cli(), *_flags */
#include <asm/uaccess.h>     /* copy_from/to_user */
#include <linux/errno.h>     /* error codes */
#include <linux/init.h>
#include <linux/fcntl.h>     /* O_ACCMODE */
#include <linux/fs.h>        /* everything... */
#include <linux/jiffies.h>
#include <linux/kernel.h>    /* printk() */
#include <linux/module.h>
#include <linux/ctype.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>      /* kmalloc() */
#include <linux/timer.h>     /* sw timers */
#include <linux/types.h>     /* size_t */
#include <asm/gpio.h>
#include <asm/arch/pxa-regs.h>
#include <linux/time.h>
#include <linux/delay.h>

#include <linux/interrupt.h>
#include <asm-arm/arch/hardware.h>
MODULE_LICENSE("Dual BSD/GPL");

#define KERNEL_BUF_LENGTH 100

// Variables
int irq17;
static int mygpio_major = 61;
static char module_name_k[] = "button"; 
char instr[10];
char* kbuf;

// Functions
static int mygpio_init(void);
static int mygpio_exit(void);
static int     mygpio_open(struct inode *inode, struct file *filp);
static int     mygpio_release(struct inode *inode, struct file *filp);
static ssize_t mygpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t mygpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static int mygpio_fasync(int fd, struct file *filp, int mode);


// Init and Exit
module_init(mygpio_init);
module_exit(mygpio_exit);

// Character Device
static const struct file_operations mygpio_fops = {
 .open = mygpio_open,
 .release = mygpio_release,
 .read = mygpio_read,
 .write = mygpio_write,
 .fasync = mygpio_fasync
};

// SIGIO
struct fasync_struct *async_queue;


// Button Interrupt
irqreturn_t gpio_irq(int irq, void *dev_id, struct pt_regs *regs)
{
 if (irq==irq17) {
    //printk("Button 1\n");
    if (async_queue) {
		//printk("ASYNC STUFF\n");
		kill_fasync(&async_queue, SIGIO, POLL_IN); }
    return IRQ_HANDLED;
  }	
}

static int mygpio_init(void)
{
	int result;
  	char routine_name_k[] = "mygpio_init";

	/* Registering device */
	result = register_chrdev(mygpio_major, "mygpio", &mygpio_fops);
	if (result < 0) {
   		printk(KERN_ALERT "%s: cannot obtain major number %d \n", module_name_k, mygpio_major);
    		return result;
  	}

	// allocating k buffer.
  	kbuf = (char*)kmalloc(KERNEL_BUF_LENGTH, GFP_KERNEL);
  
 	 // error check
	if (!kbuf) { 
	    	printk(KERN_ALERT "%s: %s: insufficient kernel memory! \n", module_name_k, routine_name_k); 
	    	result = -ENOMEM;
	    	goto fail; 
  	} 
	
 	memset(kbuf, 0, KERNEL_BUF_LENGTH);

 	pxa_gpio_mode(17 | GPIO_IN);           // BTN0 reseting system.

	
 
 	//init irq
 
  	irq17 = IRQ_GPIO(17);
	if (request_irq(irq17, &gpio_irq, SA_INTERRUPT | SA_TRIGGER_RISING,"mygpio1", NULL) != 0 ) {
                printk ( "irq not acquired \n" );
                return -1;
        }

	return 0;

fail: 
	mygpio_exit(); 
	return result;
}


static int mygpio_exit(void){
	/* unregister the device */
	unregister_chrdev(mygpio_major, "mygpio");

	free_irq(IRQ_GPIO(17), NULL);

	/* Freeing buffer memory */
	if(kbuf)
	{
		kfree(kbuf);
	}
 	printk(KERN_ALERT "Removing button module....\n");
 	return 0;
}

// Logistics to ensure module works
static int mygpio_open(struct inode *inode, struct file *filp) { return 0; }
static int mygpio_release(struct inode *inode, struct file *filp) { 
	mygpio_fasync(-1, filp, 0);
	return 0; 
}
static ssize_t mygpio_read(struct file *filp, char *ubuf, size_t count, loff_t *f_pos) { return 0; }
static ssize_t mygpio_write(struct file *filp, const char *ubuf, size_t count, loff_t *f_pos) {	return 0; }
static int mygpio_fasync(int fd, struct file *filp, int mode) {
	return fasync_helper(fd, filp, mode, &async_queue);
}


