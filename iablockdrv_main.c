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

static int __init iablockdrv_init(void)
{
	pr_info("Module loaded...\n");
	return 0;
}

static void __exit iablockdrv_exit(void)
{
	pr_info("Module unloaded...\n");
}

module_init(iablockdrv_init);
module_exit(iablockdrv_exit);

MODULE_LICENSE("GPL");

