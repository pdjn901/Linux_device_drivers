/*
This code is simply used for user space testing, 
  Here, simply we are calling kernel space /dev/spio_0.0 
  And utilising for our user space spi communications. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0" // SPI device file
#define SPI_SPEED 1000000            // SPI speed in Hz

int spi_fd;

void spi_init() {
    spi_fd = open(SPI_DEVICE, O_RDWR);
    if (spi_fd < 0) {
        perror("Failed to open SPI device");
        exit(1);
    }

    // Set SPI mode
    int mode = SPI_MODE_0;
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Failed to set SPI mode");
        exit(1);
    }

    // Set SPI speed
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &SPI_SPEED) < 0) {
        perror("Failed to set SPI speed");
        exit(1);
    }
}

void spi_transfer(unsigned char *tx_buf, unsigned char *rx_buf, size_t len) {
    struct spi_ioc_transfer transfer = {
        .tx_buf = (unsigned long)tx_buf,
        .rx_buf = (unsigned long)rx_buf,
        .len = len,
    };

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        perror("SPI transfer error");
        exit(1);
    }
}

int main() {
    spi_init();

    // Example write and read buffers
    unsigned char tx_buffer[] = {0x01, 0x02, 0x03};
    unsigned char rx_buffer[3];

    // Perform SPI transfer
    spi_transfer(tx_buffer, rx_buffer, sizeof(tx_buffer));

    // Print received data
    for (int i = 0; i < sizeof(rx_buffer); i++) {
        printf("Received: 0x%02X\n", rx_buffer[i]);
    }

    close(spi_fd);
    return 0;
}
