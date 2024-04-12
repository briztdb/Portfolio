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

#include <linux/uaccess.h>	/* copy_*_user */

#include "scull.h"		/* local definitions */
#include <linux/list.h>
#include <linux/mutex.h>
/*
 * Our parameters which can be set at load time.
 */

static int scull_major =   SCULL_MAJOR;
static int scull_minor =   0;
static int scull_quantum = SCULL_QUANTUM;

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_quantum, int, S_IRUGO);

MODULE_AUTHOR("briztdb");
MODULE_LICENSE("Dual BSD/GPL");

static DEFINE_MUTEX(mux);
static LIST_HEAD(cache_head);

static struct cdev scull_cdev;		/* Char device structure */

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

struct cache { // struct to represent cache
    struct list_head list;
    pid_t pid;
    pid_t tgid;
};
// critical section of adding a node to the cache
static void add_node_critical(struct cache* cache) { // critical section of adding node to cache
    list_add_tail(&cache->list, &cache_head);
}
// wrapper function to add a node to the cache
int add_node(pid_t pid, pid_t tgid) {
    struct cache* cache = kmalloc(sizeof(*cache), GFP_KERNEL);
    if(cache == NULL) {
	printk(KERN_ERR "Error allocating space for new node");
	return -1;
    }
    cache->pid = pid;
    cache->tgid = tgid;
    mutex_lock(&mux);
    add_node_critical(cache); // critical section
    mutex_unlock(&mux);
    return 0;
}

static int find_node_critical(pid_t pid, pid_t tgid) { // critical section of searching for a node in the cache. returns 0 on cache miss, 1 on hit
    struct cache* temp; // set up iterator
    list_for_each_entry(temp, &cache_head, list){ // built-in function to iterate through list
	if(temp->pid == pid && temp->tgid == tgid) { // if pid and tgid are the same, node already exists, return 1
	    return 1;
	}
    }
    return 0;
}

// wrapper function to find node in the cache. returns 0 on cache miss, 1 on hit.
int find_node(pid_t pid, pid_t tgid) {
    int retval;
    mutex_lock(&mux);
    retval = find_node_critical(pid, tgid); // critical section
    mutex_unlock(&mux);
    return retval;
}

/*
 * The ioctl() implementation
 */

static long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int err = 0, tmp;
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

	case SCULL_IOCRESET:
		scull_quantum = SCULL_QUANTUM;
		break;

	case SCULL_IOCSQUANTUM: /* Set: arg points to the value */
		retval = __get_user(scull_quantum, (int __user *)arg);
		break;

	case SCULL_IOCTQUANTUM: /* Tell: arg is the value */
		scull_quantum = arg;
		break;

	case SCULL_IOCGQUANTUM: /* Get: arg is pointer to result */
		retval = __put_user(scull_quantum, (int __user *)arg);
		break;

	case SCULL_IOCIQUANTUM: /* Return task_info struct */
	    struct task_info t_info;
	    t_info = (struct task_info){ // create task_info struct and fill with task information
		.__state = current->__state,
		.cpu = current->thread_info.cpu,
		.prio = current->prio,
		.pid = current->pid,
		.tgid = current->tgid,
		.nvcsw = current->nvcsw,
		.nivcsw = current->nivcsw,
	    };
	    if(copy_to_user((struct task_info __user *)arg, &t_info, sizeof(struct task_info)) != 0) { 
		return -1;
	    }
	    if(find_node(current->pid, current->tgid) == 0) { // check if node is already in cache, if not add it
		add_node(current->pid, current->tgid);
	    }
	    retval = 0;
	    break;
	case SCULL_IOCQQUANTUM: /* Query: return it (it's positive) */
		return scull_quantum;

	case SCULL_IOCXQUANTUM: /* eXchange: use arg as pointer */
		tmp = scull_quantum;
		retval = __get_user(scull_quantum, (int __user *)arg);
		if (retval == 0)
			retval = __put_user(tmp, (int __user *)arg);
		break;

	case SCULL_IOCHQUANTUM: /* sHift: like Tell + Query */
		tmp = scull_quantum;
		scull_quantum = arg;
		return tmp;

	default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	return retval;
}

struct file_operations scull_fops = {
	.owner =    THIS_MODULE,
	.unlocked_ioctl = scull_ioctl,
	.open =     scull_open,
	.release =  scull_release,
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

	struct list_head* cursor;
	struct list_head* temp_head;
	struct cache* temp_cache_node;
	int count = 1;
	mutex_lock(&mux); // start critical section
	list_for_each_safe(cursor, temp_head, &cache_head) { // iterate through cache using list_for_each_safe so we can delete elements
	    temp_cache_node = list_entry(cursor, struct cache, list); // function which returns current entry pointed to by cursor
	    printk(KERN_INFO "Task %d: PID: %d, TGID: %d\n", count++, temp_cache_node->pid, temp_cache_node->tgid);
	    list_del(cursor); // function which deletes node pointed to by cursor
	    kfree(temp_cache_node); // kernel free
	}
	mutex_unlock(&mux); // end of critical section
	
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

	return 0; /* succeed */

  fail:
	scull_cleanup_module();
	return result;
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);
