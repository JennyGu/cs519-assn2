# iablockdrv Makefile
# by William A. Katsak <wkatsak@cs.rutgers.edu>
# for CS 519, Fall 2012, Rutgers University

obj-m	:= iablockdrv.o
# If you add any more source files, you must amend the following line:
iablockdrv-objs := iablockdrv_main.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

default: build

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.* modules.* dist

