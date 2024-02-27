//can protocol
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

#define DEV_NAME "mywifidevice"
#define MAJOR_NUM 251
#define MINOR_NUM 0
#define NUM_MINORS 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Wi-Fi Protocol Device Driver");

static dev_t dev;
static struct cdev cdev;

static int wifi_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Wi-Fi device opened\n");
    // Add code to initialize Wi-Fi device
    return 0;
}

static int wifi_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Wi-Fi device closed\n");
    // Add code to release Wi-Fi device
    return 0;
}

static ssize_t wifi_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    // Add code to handle read operations
    return 0;
}

static ssize_t wifi_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    // Add code to handle write operations
    return len;
}

static struct file_operations wifi_fops = {
    .owner = THIS_MODULE,
    .open = wifi_open,
    .release = wifi_release,
    .read = wifi_read,
    .write = wifi_write,
};

static int __init wifi_init(void) {
    int ret;
    dev = MKDEV(MAJOR_NUM, MINOR_NUM);
    ret = register_chrdev_region(dev, NUM_MINORS, DEV_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register device\n");
        return ret;
    }
    cdev_init(&cdev, &wifi_fops);
    cdev.owner = THIS_MODULE;
    ret = cdev_add(&cdev, dev, NUM_MINORS);
    if (ret < 0) {
        unregister_chrdev_region(dev, NUM_MINORS);
        printk(KERN_ERR "Failed to add device\n");
        return ret;
    }
    printk(KERN_INFO "Wi-Fi device driver initialized\n");
    return 0;
}

static void __exit wifi_exit(void) {
    cdev_del(&cdev);
    unregister_chrdev_region(dev, NUM_MINORS);
    printk(KERN_INFO "Wi-Fi device driver exited\n");
}

module_init(wifi_init);
module_exit(wifi_exit);
/*
How to create /dev/wi-fi file 
1.$cat /proc/myWi-FiFile. 
Note you can see major number and minor number here. 

$sudo mknod /dev/mywifidevice c 251 0

*/
