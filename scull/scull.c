#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include "scull.h"

/* Parameters */
int scull_major = SCULL_MAJOR; // major number
int scull_minor = 0; // minor numer
int scull_num_dev = SCULL_NUM_DEV; // number of bare scull devices
int scull_quantum = SCULL_QUANTUM;
int scull_qset = SCULL_QSET;

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_num_dev, int, S_IRUGO);
module_param(scull_quantum, int, S_IRUGO);
module_param(scull_qset, int, S_IRUGO);

MODULE_LICENSE("Dual BSD/GPD");

/* Define scull devices */
struct scull_dev *scull_devices; // allocated in scull_init_module

// Trim
int scull_trim(struct scull_dev *dev) {
	struct scull_qset *next, *dptr;
	int qset = dev->qset; // "dev" is not null
	int i;

	for(dptr = dev->data; dptr; dptr = next) { // all the list items
		if(dptr->data) {
			for(i = 0; i < qset; i++)
				kfree(dptr->data[i]);
			kfree(dptr->data);
			dptr->data = NULL;
		}
		next = dptr->next;
		kfree(dptr);
	}

	dev->size = 0;
	dev->quantum = scull_quantum;
	dev->qset = scull_qset;
	dev->data = NULL;

	return 0;
}

// Open
int scull_open(struct inode *inode, struct file *filp) {
	struct scull_dev *dev; // device information

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev; // for other methods
	
	// now trim to 0 to the length of the device if open was write-only
	if((filp->f_flags & O_ACCMODE) == O_WRONLY) {
		scull_trim(dev); // ignore errors
	}
	
	return 0; // success
}

// Release
int scull_release(struct inode *inode, struct file *filp) {
	return 0;
}

// Follow the list
static struct scull_qset *scull_follow(struct scull_dev *dev, int n) {
	struct scull_qset *qs = dev->data;

	// Allocate first qset explicitly if need be
	if(!qs) {
		qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
		if(qs == NULL)
			return NULL; // never mind

		memset(qs, 0, sizeof(struct scull_qset));
	}

	// Then follow the list
	while(n--) {
		if(!qs->next) {
			qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
			if(qs->next == NULL)
				return NULL; // never mind

			memset(qs->next, 0, sizeof(struct scull_qset));
		}
		qs = qs->next;
		continue;
	}
	
	return qs;
}

// Read
ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
	struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr; // the first list item
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset; // how many bytes in the listitem
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;

	if(down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	if(*f_pos >= dev->size)
		goto out;
	if(*f_pos + count > dev->size)
		count = dev->size - *f_pos;

	// find listitem, qset index, and offset in the quantum
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	s_pos = rest / quantum;
	q_pos = rest % quantum;

	// follow the list up to the right position (defined elsewhere)
	dptr = scull_follow(dev, item);

	if (dptr == NULL || !dptr->data || !dptr->data[s_pos])
		goto out; // don't fill holes

	// read only uip to the end of this quantum
	if (count > quantum - q_pos)
		count = quantum - q_pos;

	if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count)) {
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;
	retval = count;
	
out:
	up(&dev->sem);
	return retval;
}

// write
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
	struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr;
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset;
	int item, s_pos, q_pos, rest;
	ssize_t retval = -ENOMEM; // value used in "goto out" statements

	if(down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	// find listitem, qset index and offset in the quantum
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	s_pos = rest / quantum;
	q_pos = rest % quantum;

	// follow the list up to the right position
	dptr = scull_follow(dev, item);
	if (dptr == NULL)
		goto out;
	
	if(!dptr->data) {
		dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
		if(!dptr->data) 
			goto out;
		memset(dptr->data, 0, qset * sizeof(char *));
	}
	if(!dptr->data[s_pos]) {
		dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
		if(!dptr->data[s_pos])
			goto out;
	}

	// write only up to the end of this quantum
	if (count > quantum - q_pos)
		count = quantum - q_pos;

	if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count)) {
		retval = -EFAULT;
		goto out;
	}

	*f_pos += count;
	retval = count;

	// update the size
	if(dev->size < *f_pos)
		dev->size = *f_pos;

out:
	up(&dev->sem);
	return retval;
}

// Change the current read/write position in a file
// New position is returnedas a positive return value
loff_t scull_llseek(struct file *filp, loff_t off, int whence) {
	struct scull_dev *dev = filp->private_data;
	loff_t newpos;

	switch(whence) {
		case 0: // SEEK_SET
			newpos = off;
			break;
		case 1: // SEEK_CUR
			newpos = filp->f_pos + off;
			break;
		case 2: // SEEK_END
			newpos = dev->size + off;
			break;

		default: // can't happen
			return -EINVAL;
	}

	if(newpos < 0) return -EINVAL;
	filp->f_pos = newpos;
	return newpos;
}

// Set file operations
struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	.open = scull_open,
	.release = scull_release
};

// Cleanup module
void scull_cleanup_module(void) {
	int i;
	dev_t devno = MKDEV(scull_major, scull_minor);

	// Get rid of our char dev entries
	if(scull_devices) {
		for(i = 0; i < scull_num_dev; i++) {
			scull_trim(scull_devices + i);
			cdev_del(&scull_devices[i].cdev);
		}
		kfree(scull_devices);
	}

	// Unregister
	// cleanup_module is never called if registering failed
	unregister_chrdev_region(devno, scull_num_dev);
}

// set cdev
static void scull_setup_cdev(struct scull_dev *dev, int index) {
	int err, devno;

	devno = MKDEV(scull_major, scull_minor + index);
	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add(&dev->cdev, devno, 1);

	// Fail gracefully if need be
	if(err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

// Initialize module
int  scull_init_module(void) {
	int result, i;
	dev_t dev = 0;

	if(scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		// Register
		result = register_chrdev_region(dev, scull_num_dev, "scull");
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, scull_num_dev, "scull");
		scull_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		return result;
	}

	/* allocate the devices, we can't have them static, as the number
	 * can be specified at load time
	 */
	scull_devices  = kmalloc(scull_num_dev * sizeof(struct scull_dev), GFP_KERNEL);
	if(!scull_devices) {
		result = -ENOMEM;
		goto fail; // Make this more graceful
	}
	memset(scull_devices, 0, scull_num_dev * sizeof(struct scull_dev));

	// Initialize each device.
	for (i = 0; i < scull_num_dev; i++) {
		scull_devices[i].quantum = scull_quantum;
		scull_devices[i].qset = scull_qset;
		sema_init(&scull_devices[i].sem, 1);
		scull_setup_cdev(&scull_devices[i], i);
	}

	return 0; // succeed
	
fail:
	scull_cleanup_module();
	return result;
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);

