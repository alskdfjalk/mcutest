obj-m := scull.o
KERNELDIR ?= /usr/src/kernels/2.6.32-431.el6.x86_64
PWD := $(shell pwd)
default:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -f *.o *.ko* modules.* Module.* *.mod.c *~
