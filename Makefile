modname := intel_backlight_fix
obj-m := $(modname).o

KVERSION := $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build
PWD := "$$(pwd)"

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) O=$(PWD) -C $(KDIR) M=$(PWD) clean

load:
	-rmmod $(modname)
	insmod $(modname).ko

install:
	mkdir -p /lib/modules/$(KVERSION)/misc/$(modname)
	install -m 0755 -o root -g root $(modname).ko /lib/modules/$(KVERSION)/misc/$(modname)
	depmod -a

uninstall:
	rm /lib/modules/$(KVERSION)/misc/$(modname)/$(modname).ko
	rmdir /lib/modules/$(KVERSION)/misc/$(modname)
	rmdir /lib/modules/$(KVERSION)/misc
	depmod -a

dkms:
	cp -au $(PWD) /usr/src/intel_backlight_fix-1.0
	cd /usr/src/intel_backlight_fix-1.0
	make clean
	dkms add intel_backlight_fix/1.0 -k $(KVERSION)
	dkms build intel_backlight_fix/1.0 -k $(KVERSION)
	dkms install intel_backlight_fix/1.0 -k $(KVERSION)

dkms_remove:
	dkms uninstall intel_backlight_fix/1.0 || true
	dkms remove intel_backlight_fix/1.0
	rm -rf /usr/src/intel_backlight_fix-1.0
