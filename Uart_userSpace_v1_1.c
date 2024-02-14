#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int main() {
    int uart_fd;
    char buffer[256];
    
    // Open the UART device file
    uart_fd = open("/dev/ttyS0", O_RDWR);
    if (uart_fd == -1) {
        perror("Error opening UART device");
        return 1;
    }
    
    // Configure UART settings (baud rate, etc.)
    struct termios uart_options;
    tcgetattr(uart_fd, &uart_options);
    cfsetospeed(&uart_options, B9600); // Set baud rate to 9600
    tcsetattr(uart_fd, TCSANOW, &uart_options);
    
    // Read from UART
    int bytes_read = read(uart_fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Error reading from UART");
        close(uart_fd);
        return 1;
    }
    
    // Display received data
    printf("Received %d bytes: %s\n", bytes_read, buffer);
    
    // Write to UART
    char tx_data[] = "Hello UART!";
    int bytes_written = write(uart_fd, tx_data, sizeof(tx_data));
    if (bytes_written == -1) {
        perror("Error writing to UART");
        close(uart_fd);
        return 1;
    }
    
    // Close UART device file
    close(uart_fd);
    
    return 0;
}
