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



static void iablockdrv_make_request(struct request_queue *q, struct bio *bio) {
   
    /* Redirect the I/O request to our target device */
    bio->bi_bdev = drv_info->target_dev;

    /* Pass it on? --- THIS IS MOST LIKELY INCORRECT */
    generic_make_request(bio);
}



static int __init iablockdrv_init(void) {
    /* Allocate kernel memory for our driver information */
    if (NULL == (drv_info = kzalloc(sizeof(struct iablockdrv_dev), GFP_KERNEL)))
        return -ENOMEM;
    
    /* Regsiter the device and request a major number */
    if (0 >= (drv_info->major = register_blkdev(0, "iablockdrv")))
        goto free; 
    
    /* Allocate a special queue to bypass queueing */
    if (NULL == (drv_info->queue = blk_alloc_queue(GFP_KERNEL)))
        goto unregister;

    /* And tell the kernel about our desire to skip queueing */
    blk_queue_make_request(drv_info->queue, iablockdrv_make_request);

    /* Try to open the target device to which our reads/writes will go */
    drv_info->target_dev = blkdev_get_by_path(target_device, 
                                              FMODE_READ|FMODE_WRITE|FMODE_EXCL,
                                              drv_info);

    if (NULL == drv_info->target_dev) 
        goto unregister;

    if (NULL == (drv_info->gd = alloc_disk(1)))
        goto unregister;

    drv_info->gd->major = drv_info->major;
    drv_info->gd->first_minor = 1;
    drv_info->gd->fops = NULL;
    drv_info->gd->queue = drv_info->queue;
    drv_info->gd->private_data = drv_info;
    add_disk(drv_info->gd);

    pr_info("Module loaded... major number: %u\n", drv_info->major);
    pr_info("\tUsing %s as a target driver\n", target_device);

    return 0;

unregister:
    unregister_blkdev(drv_info->major, "iablockdrv");
free:
    kfree(drv_info);

    return -ENOMEM;
}



static void __exit iablockdrv_exit(void)
{
    blkdev_put(drv_info->target_dev, FMODE_READ|FMODE_WRITE|FMODE_EXCL);

    
    del_gendisk(drv_info->gd);
    blk_cleanup_queue(drv_info->queue);
    put_disk(drv_info->gd);
    unregister_blkdev(drv_info->major, "iablockdrv");
    kfree(drv_info);

    pr_info("Module unloaded...\n");
}

module_init(iablockdrv_init);
module_exit(iablockdrv_exit);

MODULE_LICENSE("GPL");
