#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#define CAN_INTERFACE "can0" // Change this to the actual CAN interface name

int main(void) {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    int nbytes;

    // Create socket
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Specify the CAN interface
    strcpy(ifr.ifr_name, CAN_INTERFACE);
    ioctl(s, SIOCGIFINDEX, &ifr);

    // Bind the socket to the CAN interface
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(s);
        return 1;
    }

    // Prepare CAN frame
    frame.can_id = 0x123; // Change this to the CAN ID you want to use
    frame.can_dlc = 8;    // Change this to the data length code (0-8 bytes)
    strcpy((char *)frame.data, "Hello!");

    // Send CAN frame
    nbytes = write(s, &frame, sizeof(struct can_frame));
    if (nbytes != sizeof(struct can_frame)) {
        perror("Write failed");
        close(s);
        return 1;
    }

    printf("Message sent: %s\n", frame.data);

    // Receive CAN frame
    nbytes = read(s, &frame, sizeof(struct can_frame));
    if (nbytes < 0) {
        perror("Read failed");
        close(s);
        return 1;
    } else if (nbytes == 0) {
        printf("No data received\n");
    } else {
        printf("Message received: %s\n", frame.data);
    }

    close(s);
    return 0;
}
