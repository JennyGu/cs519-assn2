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

static int __init iablockdrv_init(void) {
    drv_info = kzalloc(sizeof(struct iablockdrv_dev), GFP_KERNEL);
    
    if (!drv_info) {
        return -ENOMEM;
    }

	pr_info("Module loaded...\n");
	pr_info("\tUsing %s as a target driver\n", target_device);

	if (NULL != target_device[0]) {
	    drv_info->target_dev = blkdev_get_by_path(target_device, 
	                                              FMODE_READ|FMODE_WRITE|FMODE_EXCL,
	                                              drv_info);
    }

    if (NULL == drv_info->target_dev) {
        return -1;
    }

	return 0;
}

static void __exit iablockdrv_exit(void)
{
	pr_info("Module unloaded...\n");
}

module_init(iablockdrv_init);
module_exit(iablockdrv_exit);

MODULE_LICENSE("GPL");

