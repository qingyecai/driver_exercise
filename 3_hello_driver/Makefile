CONFIG_MODULE_SIG=n
obj-m := hello.o

KERNEL_DIR := /lib/modules/$(shell uname -r)/build
													#指定内核源码
PWD := $(shell pwd)
					#指向当前目录

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean