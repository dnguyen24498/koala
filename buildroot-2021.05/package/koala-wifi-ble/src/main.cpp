#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>

#define UART_DEVICE "/dev/ttyS1"
#define BAUDRATE B115200

int main() {
    int uart_fd;
    struct termios options;
    const char *message = "Hello, UART!\n";

    // Open the UART device
    uart_fd = open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("Unable to open UART");
        return errno;
    }

    // Configure the UART
    tcgetattr(uart_fd, &options); // Get current options
    cfsetispeed(&options, BAUDRATE); // Set input baud rate
    cfsetospeed(&options, BAUDRATE); // Set output baud rate

    options.c_cflag |= (CLOCAL | CREAD); // Ignore modem control lines and enable receiver
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // One stop bit
    options.c_cflag &= ~CSIZE; // Clear the current character size setting
    options.c_cflag |= CS8; // 8 data bits

    // Apply the configuration
    tcsetattr(uart_fd, TCSANOW, &options);

    // Send a string every second
    while (1) {
        ssize_t bytes_written = write(uart_fd, message, strlen(message));
        if (bytes_written < 0) {
            perror("Failed to write to UART");
            close(uart_fd);
            return errno;
        }
        printf("Sent: %s", message);
        sleep(1); // Wait for 1 second
    }

    // Close the UART (unreachable in this example)
    close(uart_fd);
    return 0;
}
