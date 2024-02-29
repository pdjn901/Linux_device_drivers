//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_FILE "/dev/myWi-Fi"

int main() {
    int fd;
    char buffer[256];

    // Open device file
    fd = open(DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device file");
        return -1;
    }

    // Example: Writing data to the device
    char data_to_send[] = "AT\r\n";
    if (write(fd, data_to_send, strlen(data_to_send)) < 0) {
        perror("Failed to write to device");
        close(fd);
        return -1;
    }

    // Example: Reading data from the device
    ssize_t num_bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (num_bytes < 0) {
        perror("Failed to read from device");
        close(fd);
        return -1;
    }

    // Null-terminate the buffer
    buffer[num_bytes] = '\0';

    // Print received data
    printf("Received from device: %s\n", buffer);

    // Close device file
    close(fd);

    return 0;
}
