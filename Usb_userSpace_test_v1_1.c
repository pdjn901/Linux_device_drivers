#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>

#define USB_DEVICE_PATH "/dev/bus/usb/001/002" // Adjust this path to match your USB device

int main() {
    int usb_fd;
    char buf[64]; // Buffer for data to send/receive

    // Open the USB device file
    usb_fd = open(USB_DEVICE_PATH, O_RDWR);
    if (usb_fd < 0) {
        perror("Failed to open USB device file");
        return 1;
    }

    // Send data to the USB device
    strcpy(buf, "Hello USB device!");
    if (write(usb_fd, buf, strlen(buf) + 1) < 0) {
        perror("Failed to write to USB device");
        close(usb_fd);
        return 1;
    }

    // Receive data from the USB device
    if (read(usb_fd, buf, sizeof(buf)) < 0) {
        perror("Failed to read from USB device");
        close(usb_fd);
        return 1;
    }

    // Print the received data
    printf("Received data: %s\n", buf);

    // Close the USB device file
    close(usb_fd);

    return 0;
}
