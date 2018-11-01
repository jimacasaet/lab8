obj-m := lab8_spinlock_macasaet.o
KDIR := /lib/modules/4.15.0-36-generic/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
 
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
