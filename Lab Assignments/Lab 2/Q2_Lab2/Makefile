obj-m+=2017csb1095_Q2.o
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C/lib/modules/$(shell uname -r)/build/ M=$(PWD) clean