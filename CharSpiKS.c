//char SPi.
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/spi/spi.h>

#define DEVICE_NAME "spi_char_dev"
#define SPI_BUS_ID 0
#define SPI_DEVICE_ID 0
#define DATA_SIZE 16  // Example data size

static dev_t dev_num;
static struct cdev *my_cdev;
static struct spi_device *my_spi_dev;

static ssize_t spi_char_read(struct file *filp, char __user *user_buf, size_t count, loff_t *f_pos) {
  char *spi_data;

  // Retrieve data from SPI device
  spi_data = spi_transfer(spi_bus_id, spi_device_id, NULL, 0, spi_data, DATA_SIZE);
  if (!spi_data) {
    dev_err(dev_get_drvdata(filp->f_dentry->d_sb), "Failed to receive data from SPI device\n");
    return -EFAULT;
  }

  // Copy data to user buffer (up to requested count)
  count = min(count, DATA_SIZE);  // Limit copy size to actual data size
  if (copy_to_user(user_buf, spi_data, count)) {
    kfree(spi_data);
    return -EFAULT;
  }

  kfree(spi_data);
  return count;
}

static const struct file_operations spi_char_fops = {
  .owner = THIS_MODULE,
  .read = spi_char_read,
};

static int __init spi_char_init(void) {
  int ret;

  // Allocate device number
  ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
  if (ret < 0)
    return ret;

  // Get SPI device (replace with appropriate initialization for your driver)
  my_spi_dev = spi_bus_find_device(spi_bus_id, spi_device_id);
  if (!my_spi_dev) {
    dev_err(&dev_get_drvdata(my_cdev->dev), "Failed to find SPI device\n");
    ret = -EOPNOTSUPP;
    goto err_free_chrdev;
  }

  // Create and register character device
  my_cdev = cdev_alloc();
  if (!my_cdev) {
    ret = -ENOMEM;
    goto err_free_chrdev;
  }
  cdev_init(my_cdev, &spi_char_fops, 1);
  ret = cdev_add(my_cdev, dev_num, 1);
  if (ret < 0)
    goto err_free_cdev;

  return 0;

err_free_cdev:
  kobject_put(&my_cdev->kobj);
err_free_chrdev:
  unregister_chrdev_region(dev_num, 1);
  return ret;
}

static void __exit spi_char_exit(void) {
  cdev_del(my_cdev);
  unregister_chrdev_region(dev_num, 1);
}

module_init(spi_char_init);
module_exit(spi_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Character driver for SPI data access");
