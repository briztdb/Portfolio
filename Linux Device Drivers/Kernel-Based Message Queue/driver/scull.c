/*
 * scull.c -- the bare scull char module
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>	/* copy_*_user */
#include "scull.h"		/* local definitions */
#include <linux/semaphore.h>

/*
 * Our parameters which can be set at load time.
 */

static int scull_major =   SCULL_MAJOR;
static int scull_minor =   0;
static int scull_fifo_elemsz = SCULL_FIFO_ELEMSZ_DEFAULT; /* ELEMSZ */
static int scull_fifo_size   = SCULL_FIFO_SIZE_DEFAULT;   /* N      */

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_fifo_size, int, S_IRUGO);
module_param(scull_fifo_elemsz, int, S_IRUGO);

MODULE_AUTHOR("briztdb");
MODULE_LICENSE("Dual BSD/GPL");

struct semaphore full, empty;
static DEFINE_MUTEX(mux);

static struct cdev scull_cdev;		/* Char device structure */
static char* msg_queue;
static char* queue_start; // start pointer 
static char* queue_end; // end pointer
static int length; // length of msg_queue in bytes
static int location_read; // counter to check if start pointer passed msg_queue bounds
static int location_write; // counter to check if end pointer passed msg_queue bounds
/*
 * Open and close
 */

static int scull_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "scull open\n");
	return 0;          /* success */
}

static int scull_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "scull close\n");
	return 0;
}

/*
 * Read and Write
 */
static ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {

   /* TODO: implement this function */
    int size = 0;
    int retval;
    char* old_start; // variable to store current start pointer
    char* temp;

    // start critical section
    if(down_interruptible(&full)) { // check if lock was interrupted, return -EINTR if so
	printk(KERN_INFO "SIGINT RECEIVED\n");
	return -EINTR;
    }
    // if count is smaller than length of next elem, only copy count chars
    if(count < *queue_start) {
	size = count;
    } else {
	size = *queue_start;
    }
    old_start = queue_start;
    temp = kzalloc(size, GFP_KERNEL); // allocate space for message buffer
    for(int j = 0; j < size; j++) { // copy message into buffer
	temp[j] = queue_start[j];
    }
    retval = copy_to_user(buf, temp, size); // send message to user
    if(retval != 0) { // if copying message to user failed, return error
	printk(KERN_INFO "Copying to user failed\n");
	return -EFAULT;
    }
    kfree(temp);
    up(&empty); // end critical section
    queue_start = (queue_start + scull_fifo_elemsz); // move read pointer to next full element
    location_read++; // int to dictate how many messages have been read since last loop-back
    if(location_read > scull_fifo_size - 1) { // if read pointer passed array bounds, loop back to beginning
	location_read = 0;
	queue_start = msg_queue;
    }
    printk(KERN_INFO "scull read\n");
    return size;
}


static ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    /* TODO: implement this function */
    int size;
    char* temp;
    int retval;

    // start critical section
    if(down_interruptible(&empty)) { // if lock is interrupted, return -EINTR
	printk(KERN_INFO "SIGINT RECEIVED\n");
	return -EINTR;
    }
    if(count > scull_fifo_elemsz) { // check size of count and if it is valid
	size = scull_fifo_elemsz;
    } else {
	size = count;
    }

    temp = kmalloc(size, GFP_KERNEL);
    retval = copy_from_user(temp, buf, size); // copy string from user space
    if(retval != 0) { // check if copy_from_user succeeded
	printk(KERN_INFO "Copy from user failed!\n");
	return -EFAULT;
    }
    for(int i = 0; i < size; i++) { // write to message queue
	queue_end[i] = temp[i];
    }
    kfree(temp);
    queue_end = (queue_end + scull_fifo_elemsz); // updating write pointer
    location_write++; 
    if(location_write > scull_fifo_size + 1) { // check if write pointer overflows
	location_write = 0;
	queue_end = msg_queue;
    }
    up(&full); // end critical section
    
    printk(KERN_INFO "scull write\n");
    return 0;
}

/*
 * The ioctl() implementation
 */
static long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	int err = 0;
	int retval = 0;
    
	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;

	err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;

	switch(cmd) {
	case SCULL_IOCGETELEMSZ:
		return scull_fifo_elemsz;

	default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	return retval;

}

struct file_operations scull_fops = {
	.owner 		= THIS_MODULE,
	.unlocked_ioctl = scull_ioctl,
	.open 		= scull_open,
	.release	= scull_release,
	.read 		= scull_read,
	.write 		= scull_write,
};

/*
 * Finally, the module stuff
 */

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void scull_cleanup_module(void)
{
	dev_t devno = MKDEV(scull_major, scull_minor);


	/* TODO: free FIFO safely here */
	mutex_lock(&mux);
	kfree(msg_queue);
	mutex_unlock(&mux);
      	/* Get rid of the char dev entry */
	cdev_del(&scull_cdev);

	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, 1);
}

int scull_init_module(void)
{
	int result;
	dev_t dev = 0;

	/*
	 * Get a range of minor numbers to work with, asking for a dynamic
	 * major unless directed otherwise at load time.
	 */
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, 1, "scull");
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, 1, "scull");
		scull_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		return result;
	}

	cdev_init(&scull_cdev, &scull_fops);
	scull_cdev.owner = THIS_MODULE;
	result = cdev_add (&scull_cdev, dev, 1);
	/* Fail gracefully if need be */
	if (result) {
		printk(KERN_NOTICE "Error %d adding scull character device", result);
		goto fail;
	}


	/* TODO: allocate FIFO correctly here */
	mutex_lock(&mux); // lock mutex for critical section
	length = (scull_fifo_size * (sizeof(ssize_t) + scull_fifo_elemsz));
	msg_queue = kzalloc(length, GFP_KERNEL);
	queue_start = msg_queue;
	queue_end = msg_queue;
	location_read = 0;
	location_write = 0;
	mutex_unlock(&mux); // release lock

	printk(KERN_INFO "scull: FIFO SIZE=%u, ELEMSZ=%u\n", scull_fifo_size, scull_fifo_elemsz);

	// initialize semaphores
	sema_init(&full, 0); 
	sema_init(&empty, scull_fifo_size);
	return 0; /* succeed */

  fail:
	scull_cleanup_module();
	return result;
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);
