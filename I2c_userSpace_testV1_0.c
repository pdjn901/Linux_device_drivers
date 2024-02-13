#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_DEVICE_PATH "/dev/i2c-1" // Change to "/dev/i2c-0" if using I2C0

int main() {
    int file;
    char *filename = I2C_DEVICE_PATH;
    int addr = 0x48; // Example I2C device address (MCP4725 DAC)

    // Open the I2C device
    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Failed to open the I2C device");
        exit(1);
    }

    // Set the I2C slave address
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        perror("Failed to set I2C slave address");
        close(file);
        exit(1);
    }

    // Example: Write data to the I2C device
    unsigned char buffer[2] = {0x00, 0x00}; // Example data to write
    if (write(file, buffer, 2) != 2) {
        perror("Failed to write to the I2C device");
        close(file);
        exit(1);
    }

    // Example: Read data from the I2C device
    if (read(file, buffer, 2) != 2) {
        perror("Failed to read from the I2C device");
        close(file);
        exit(1);
    }

    // Print received data
    printf("Received data: 0x%02x 0x%02x\n", buffer[0], buffer[1]);

    // Close the I2C device
    close(file);

    return 0;
}
