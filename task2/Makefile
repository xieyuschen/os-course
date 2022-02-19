BASEINCLUDE ?= /lib/modules/5.8.0-44-generic/build

DevFIFOWithBlock-objs := FIFOWithBlock.o
obj-m := DevFIFOWithBlock.o

CURRENT_PATH :=$(shell pwd)
VERSION_NUM :=$(shell uname -r)
LIUNX_PATH :=/usr/src/linux-headers-$(VERSION_NUM)

all:
	make -C $(LIUNX_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(LIUNX_PATH) M=$(CURRENT_PATH) clean
