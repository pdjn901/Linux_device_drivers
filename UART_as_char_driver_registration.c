#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/serial_core.h>

// Define your character device operations
static struct file_operations uart_fops = {
    .owner = THIS_MODULE,
    .read = /* Your read function */,
    .write = /* Your write function */,
    .open = /* Your open function */,
    .release = /* Your release function */,
};

static struct uart_driver my_uart_driver = {
    .driver_name = "my_uart",
    .dev_name = "ttyMYUART", // Choose a suitable device name
    .nr = 1, // Number of UART devices handled by this driver
    .cons = NULL, // Console UART (if any)
};

static int __init my_uart_init(void) {
    int ret;

    // Register the UART driver
    ret = uart_register_driver(&my_uart_driver);
    if (ret) {
        pr_err("Failed to register UART driver: %d\n", ret);
        return ret;
    }

    // Register your character device
    // (use register_chrdev() or cdev_add() as appropriate)
    // Example:
    // ret = register_chrdev(42, "my_uart_chardev", &uart_fops);
    // if (ret) {
    //     pr_err("Failed to register character device: %d\n", ret);
    //     uart_unregister_driver(&my_uart_driver);
    //     return ret;
    // }
   //create sysfs class entry in "/sys/class/bus/UART" by using class_create(static struct_class *my_class, ,"driver_name");
  // create device file in /dev/tty/<ttySoft> by using device_create(static struct_device *my_device, ,"driver_name");
  //file_operations such as , i)uart_init(probe)-->open(), ii)copy_from_user() in ker_space-->write() in user space, iii)copy_to_user()-->read(), iv)uart_exit()

    pr_info("UART driver initialized successfully!\n");
    return 0;
}

static void __exit my_uart_exit(void) {
    // Unregister the UART driver
    uart_unregister_driver(&my_uart_driver);

    // Unregister your character device
    // (use unregister_chrdev() or cdev_del() as appropriate)

    pr_info("UART driver exited.\n");
}

module_init(my_uart_init);
module_exit(my_uart_exit);
MODULE_LICENSE("GPL");
