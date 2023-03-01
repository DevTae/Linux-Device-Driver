/* sbull.h - definitions for the char module */
#include <linux/ioctl.h>

#ifdef SBULL_MULTIQUEUE
#	warning "Multiqueue only works on 2.4 kernels"
#endif

#define SBULL_MAJOR 0		/* dynamic major by default */
#define SBULL_DEVS 2		/* two disks */
#define SBULL_RAHEAD 2		/* two sectors */
#define SBULL_SIZE 2048		/* two megs each */
#define SBULL_BLKSIZE 1024	/* 1k blocks */
#define SBULL_HARDSECT 512	/* 2.2 and 2.4 can used different values */

#define SBULLR_MAJOR 0		/* Dynamic major for raw device */

/*
 * The sbull device is removable: if it is left closed for more than
 * half a minute, it is removed. Thus use a usage count and a
 * kernel timer
 */

typedef struct sbull_dev {
	int size;
	int usage;
	struct timer_list timer;
	spinlock_t lock;
	u8 *data;
#ifdef SBULL_MULTIQUEUE
	request_queue_t *queue;
	int busy;
#endif
} sbull_dev;
