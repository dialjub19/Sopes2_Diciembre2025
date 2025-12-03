#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

#define BAR_WIDTH 30

static int ram_bar_show(struct seq_file *m, void *v)
{
    struct sysinfo si;
    unsigned long total, free, used;
    int percent, filled, i;

    si_meminfo(&si);

    total = si.totalram * si.mem_unit;
    free  = si.freeram  * si.mem_unit;
    used  = total - free;

    percent = (used * 100) / total;
    filled = (percent * BAR_WIDTH) / 100;

    seq_puts(m, "[");

    for (i = 0; i < filled; i++)
        seq_putc(m, '#');
    for (i = filled; i < BAR_WIDTH; i++)
        seq_putc(m, ' ');

    seq_printf(m, "] %d%%\n", percent);

    return 0;
}

static int ram_bar_open(struct inode *inode, struct file *file)
{
    return single_open(file, ram_bar_show, NULL);
}

static const struct proc_ops ram_bar_ops = {
    .proc_open    = ram_bar_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init ram_bar_init(void)
{
    proc_create("ram_bar", 0, NULL, &ram_bar_ops);
    printk(KERN_INFO "Modulo ram_bar cargado.\n");
    return 0;
}

static void __exit ram_bar_exit(void)
{
    remove_proc_entry("ram_bar", NULL);
    printk(KERN_INFO "Modulo ram_bar eliminado.\n");
}

module_init(ram_bar_init);
module_exit(ram_bar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SOPES2");
MODULE_DESCRIPTION("Muestra una barra ASCII del uso de RAM");
