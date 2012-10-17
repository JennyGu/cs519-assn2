/* iablockdrv main file */
/* iablockdrv_main.c */
/* by William A. Katsak <wkatsak@cs.rutgers.edu> */
/* for CS 519, Fall 2012, Rutgers University */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/hdreg.h>
#include "iablockdrv.h"

static char *target_device = "\0";
module_param(target_device, charp, 0000);

struct iablockdrv_dev {
    int major;
    struct request_queue *queue;
    struct gendisk *gd;
    struct block_device *target_dev;
};

static struct iablockdrv_dev *drv_info;

static int iablockdrv_make_request(struct request_queue *q, struct bio *bio) {
   
    /* Redirect the I/O request to our target device */
    bio->bi_bdev = drv_info->target_dev;

    /* A non-zero return value causes the request to be reissued */
    return 1;
}

static int __init iablockdrv_init(void) {
    drv_info = kzalloc(sizeof(struct iablockdrv_dev), GFP_KERNEL);
    drv_info-> target_dev = NULL;
    
    if (!drv_info) {
        return -ENOMEM;
    }

	drv_info->major = register_blkdev(0, "iablockdrv");
    if (drv_info->major <= 0) {
        return -EBUSY;
    }



	if (NULL != target_device[0]) {
	    drv_info->target_dev = blkdev_get_by_path(target_device, 
	                                              FMODE_READ|FMODE_WRITE|FMODE_EXCL,
	                                              drv_info);
    }

    if (NULL == drv_info->target_dev) {
        return -1;
    }

	pr_info("Module loaded... major number: %u\n", drv_info->major);
	pr_info("\tUsing %s as a target driver\n", target_device);

	return 0;
}

static void __exit iablockdrv_exit(void)
{
    blkdev_put(drv_info->target_dev, FMODE_READ|FMODE_WRITE|FMODE_EXCL);

    unregister_blkdev(drv_info->major, "iablockdrv");
    kfree(drv_info);

	pr_info("Module unloaded...\n");
}

module_init(iablockdrv_init);
module_exit(iablockdrv_exit);

MODULE_LICENSE("GPL");
