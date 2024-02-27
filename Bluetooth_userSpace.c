//this is simply userspace bluetooth program. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define MAX_DEVICES 10

int main(void) {
    inquiry_info *devices = NULL;
    int max_devices = MAX_DEVICES;
    int num_devices, i;

    // Open Bluetooth socket
    int sock = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
    if (sock < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Start device discovery
    num_devices = hci_inquiry(hci_get_route(NULL), 8, max_devices, NULL, &devices, IREQ_CACHE_FLUSH);
    if (num_devices < 0) {
        perror("Device discovery failed");
        close(sock);
        return EXIT_FAILURE;
    }

    printf("Found %d devices:\n", num_devices);
    for (i = 0; i < num_devices; i++) {
        char addr_str[18];
        ba2str(&(devices + i)->bdaddr, addr_str);
        printf("%s\n", addr_str);
    }

    // Free resources
    free(devices);
    close(sock);
    return EXIT_SUCCESS;
}
/*

Commands to compile

$gcc -o bluetooth_scan bluetooth_scan.c -lbluetooth
$sudo ./bluetooth_scan

*/
