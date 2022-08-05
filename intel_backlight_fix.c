
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/backlight.h>

MODULE_LICENSE("GPL");

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	// printk(KERN_INFO "intel_backlight_device_update_status: p->addr = 0x%p, ip = %lx,"
			// " flags = 0x%lx, rdi = %lx, lev=%d\n",
		// p->addr, regs->ip, regs->flags, regs->di, regs->di ? *(int *)regs->di : -1);
	struct backlight_device *bd = (struct backlight_device *)regs->di;

	if (bd != NULL && bd->props.brightness == 0) {
		printk(KERN_INFO "intel_backlight_fix: overriding brightness 0 -> 1\n");
		bd->props.brightness = 1;
	}

	return 0;
}

static struct kprobe kp = {
	.symbol_name = "intel_backlight_device_update_status",
	.pre_handler = handler_pre
};

static int intel_backlight_fix_init(void) {
	int ret;

    ret = register_kprobe(&kp);
	if (ret < 0) {
		printk(KERN_INFO "[-] unable to register kprobe, returned %d\n", ret);
		return ret;
	}

	printk(KERN_INFO "intel_backlight_fix entered\n");
	return 0;
}

module_init(intel_backlight_fix_init);

static void intel_backlight_fix_exit(void) {
	unregister_kprobe(&kp);
	printk(KERN_INFO "intel_backlight_fix leaving\n");
}

module_exit(intel_backlight_fix_exit);
