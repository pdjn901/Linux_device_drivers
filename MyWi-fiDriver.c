//my Wi-Fi driver. 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>

#define DRIVER_NAME "esp32_wifi"

static dev_t dev_num;
static struct class *esp32_class;
static struct device *esp32_device;

// Function to handle file open
static int esp32_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Opened /dev/myWi-Fi device\n");
    return 0;
}

// Function to handle file close
static int esp32_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Closed /dev/my-Wi-Fi device\n");
    return 0;
}

// File operations
static struct file_operations esp32_fops = {
    .open = esp32_open,
    .release = esp32_release,
};

static int __init esp32_init(void) {
    int ret;

    // Allocate device number
    ret = alloc_chrdev_region(&dev_num, 0, 1, DRIVER_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Failed to allocate device number\n");
        return ret;
    }

    // Create device class
    esp32_class = class_create(THIS_MODULE, DRIVER_NAME);
    if (IS_ERR(esp32_class)) {
        printk(KERN_ERR "Failed to create device class\n");
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(esp32_class);
    }

    // Create device
    esp32_device = device_create(esp32_class, NULL, dev_num, NULL, DRIVER_NAME);
    if (IS_ERR(esp32_device)) {
        printk(KERN_ERR "Failed to create device\n");
        class_destroy(esp32_class);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(esp32_device);
    }

    // Register file operations
    cdev_init(&cdev, &esp32_fops);
    ret = cdev_add(&cdev, dev_num, 1);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register file operations\n");
        device_destroy(esp32_class, dev_num);
        class_destroy(esp32_class);
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    printk(KERN_INFO "ESP32 Wi-Fi module initialized\n");
    return 0;
}

static void __exit esp32_exit(void) {
    // Unregister file operations
    cdev_del(&cdev);

    // Destroy device
    device_destroy(esp32_class, dev_num);

    // Destroy device class
    class_destroy(esp32_class);

    // Unregister device number
    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "ESP32 Wi-Fi module removed\n");
}

module_init(esp32_init);
module_exit(esp32_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("ESP32 Wi-Fi module driver");


//to create one file in /dev/
//cat /proc/
//mknod /dev/myWi-Fi 245 0.
