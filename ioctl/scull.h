/* File name */
#ifndef _SCULL_H
#define _SCULL_H

/* Dynamic major number by default */
#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0
#endif

/* scull0 to scull3 */
#ifndef SCULL_NUM_DEV
#define SCULL_NUM_DEV 4
#endif

/* Memory area */
#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

/* Array of memory area */
#ifndef SCULL_QSET
#define SCULL_QSET 1000
#endif

/* Define scull quantum set */
struct scull_qset {
	void **data;
	struct scull_qset *next;
};

/* Define scull devices */
struct scull_dev {
	struct scull_qset *data;	/* Pointer to first quantum set */
	int quantum;				/* the current quantum size */
	int qset;					/* the current array size */
	unsigned long size;			/* amount of data stored here */
	unsigned int access_key;	/* used by sculluid and scullpriv */
	struct cdev cdev;			/* char device structure */
	struct semaphore sem;		/* mutual exclusion semaphore */
};

/* Global variables (main.c) */
extern int scull_major;
extern int scull_num_dev;
extern int scull_quantum;
extern int scull_qset;

/* Prototypes for shared functions */
int scull_open(struct inode *inode, struct file *filp);
int scull_release(struct inode *inode, struct file *filp);
int scull_trim(struct scull_dev *dev);
ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
loff_t scull_llseek(struct file *filp, loff_t off, int whence);
long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

#endif
