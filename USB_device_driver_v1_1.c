#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static int my_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "USB device (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
    return 0; // Success
}

static void my_usb_disconnect(struct usb_interface *interface)
{
    struct usb_device *dev = interface_to_usbdev(interface);
    printk(KERN_INFO "USB device (%04X:%04X) unplugged\n", dev->descriptor.idVendor, dev->descriptor.idProduct);
}

static struct usb_device_id my_usb_table[] = {
    { USB_DEVICE(0x1234, 0x5678) }, // Example: Vendor ID and Product ID
    {} // Terminating entry
};
MODULE_DEVICE_TABLE(usb, my_usb_table);

static struct usb_driver my_usb_driver = {
    .name = "my_usb_driver",
    .probe = my_usb_probe,
    .disconnect = my_usb_disconnect,
    .id_table = my_usb_table,
};

static int __init my_usb_init(void)
{
    return usb_register(&my_usb_driver);
}

static void __exit my_usb_exit(void)
{
    usb_deregister(&my_usb_driver);
}

module_init(my_usb_init);
module_exit(my_usb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple USB Driver");
