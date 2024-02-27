#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/can.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define DEV_NAME "mycandevice"
#define MAJOR_NUM 250
#define MINOR_NUM 0
#define NUM_MINORS 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("CAN Protocol Device Driver");

static dev_t dev;
static struct cdev cdev;

static int can_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "CAN device opened\n");
    return 0;
}

static int can_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "CAN device closed\n");
    return 0;
}

static ssize_t can_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    // Implement read functionality
    return 0;
}

static ssize_t can_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    // Implement write functionality
    return len;
}

static struct file_operations can_fops = {
    .owner = THIS_MODULE,
    .open = can_open,
    .release = can_release,
    .read = can_read,
    .write = can_write,
};

static int __init can_init(void) {
    int ret;
    dev = MKDEV(MAJOR_NUM, MINOR_NUM);
    ret = register_chrdev_region(dev, NUM_MINORS, DEV_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register device\n");
        return ret;
    }
    cdev_init(&cdev, &can_fops);
    cdev.owner = THIS_MODULE;
    ret = cdev_add(&cdev, dev, NUM_MINORS);
    if (ret < 0) {
        unregister_chrdev_region(dev, NUM_MINORS);
        printk(KERN_ERR "Failed to add device\n");
        return ret;
    }
    printk(KERN_INFO "CAN device driver initialized\n");
    return 0;
}

static void __exit can_exit(void) {
    cdev_del(&cdev);
    unregister_chrdev_region(dev, NUM_MINORS);
    printk(KERN_INFO "CAN device driver exited\n");
}

module_init(can_init);
module_exit(can_exit);

//how to create dev file in /dev folder
//cat proc/devices      
//so you can find the MyCanDevice major number and minor number. 

/*
 use the mknod command in the terminal. Here's how you can create the device file:

bash
Copy code
sudo mknod /dev/mycandevice c 250 0
This command creates a device file named mycandevice with the major number 250 and minor number 0. Make sure to replace 250 with the actual major number assigned to your device driver.

*/
