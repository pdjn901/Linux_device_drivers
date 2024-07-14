//========================sending code==========
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/icmpv6.h>

#define MY_NETDEV_NAME "my_netdev"
#define DESTINATION_IP "192.168.0.20"  // Replace with desired destination IP

int main() 
{
  int sockfd;
  struct sockaddr_ll socket_address;
  struct ether_header *eth_header;
  struct ip *ip_header;
  struct icmpv6hdr *icmpv6_header;
  char *packet;  // Pointer to constructed packet data

  // Create a raw socket
  sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockfd == -1) {
    perror("socket");
    exit(1);
  }

  // Set up socket address for my_netdev
  memset(&socket_address, 0, sizeof(socket_address));
  socket_address.sll_family = AF_PACKET;
  socket_address.sll_halen = ETH_ALEN;
  memcpy(socket_address.sll_addr, my_netdev_mac, ETH_ALEN);  // Replace with actual MAC address if needed
  socket_address.sll_ifindex = if_nametoindex(MY_NETDEV_NAME);  // Get interface index

  // Allocate memory for the packet (Ethernet + IP + ICMPv6 headers + payload)
  packet = malloc(sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct icmpv6hdr) + 64);  // Adjust payload size as needed

  // Construct Ethernet header (replace source MAC if needed)
  eth_header = (struct ether_header *)packet;
  memcpy(eth_header->h_dest, "\xFF\xFF\xFF\xFF\xFF\xFF", ETH_ALEN);  // Broadcast destination
  memcpy(eth_header->h_source, my_netdev_mac, ETH_ALEN);  // Replace with your MAC address
  eth_header->h_proto = htons(ETH_P_IP);

  // Construct IP header
  ip_header = (struct ip *)(packet + sizeof(struct ether_header));
  ip_header->version = 4;
  ip_header->ihl = 5;
  ip_header->tos = 0;
  ip_header->tot_len = sizeof(struct ip) + sizeof(struct icmpv6hdr) + 64;  // Adjust as needed
  ip_header->id = 0; // Replace with unique identifier if needed
  ip_header->frag_off = 0;
  ip_header->ttl = 64;
  ip_header->protocol = IPPROTO_ICMPV6;
  ip_header->saddr = inet_addr("192.168.1.10");  // Replace with your source IP
  ip_header->daddr = inet_addr(DESTINATION_IP);
  ip_header->check = 0;   // Calculate checksum later (before sending)
  ip_header->check = in_checksum((unsigned short *)ip_header, sizeof(struct ip));

  // Construct ICMPv6 echo request header
  icmpv6_header = (struct icmpv6hdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
  icmpv6_header->type = ICMPV6_ECHO_REQUEST;
  icmpv6_header->code = 0;
  // You can add an ICMPv6 payload here (optional)



  // Send the packet
  if (sendto(sockfd, packet, sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct icmpv6hdr) + 64, 0,
            (struct sockaddr *)&socket_address, sizeof(socket_address)) == -1) {
    perror("sendto");
    exit(1);
  }

  printf("Sent ICMPv6 echo request to %s\n", DESTINATION_IP
  }
  
  
  //===============receiving code========
  /*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/icmpv6.h>

#define MY_NETDEV_NAME "my_netdev"
#define DESTINATION_IP "192.168.0.20"  // Replace with desired destination IP

int main() {
  int sockfd;
  struct sockaddr_ll socket_address;
  struct sockaddr_in from_addr;
  socklen_t from_len;
  ssize_t bytes_received;
  struct ether_header *eth_header;
  struct ip *ip_header;
  struct icmpv6hdr *icmpv6_header;
  char *packet_buffer;  // Buffer to receive packets

  // Create a raw socket
  sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockfd == -1) {
    perror("socket");
    exit(1);
  }

  // Set up socket address for my_netdev
  memset(&socket_address, 0, sizeof(socket_address));
  socket_address.sll_family = AF_PACKET;
  socket_address.sll_halen = ETH_ALEN;
  memcpy(socket_address.sll_addr, my_netdev_mac, ETH_ALEN);  // Replace with actual MAC address if needed
  socket_address.sll_ifindex = if_nametoindex(MY_NETDEV_NAME);  // Get interface index

  // Allocate memory for the received packet
  packet_buffer = malloc(2048);  // Adjust buffer size as needed

  while (1) {
    // Construct and send ICMPv6 echo request
    printf("Sending ICMPv6 echo request to %s\n", DESTINATION_IP);

    // ... (packet construction and sendto code from previous example)

    // Receive response
    from_len = sizeof(from_addr);
    bytes_received = recvfrom(sockfd, packet_buffer, 2048, 0, (struct sockaddr *)&from_addr, &from_len);
    if (bytes_received == -1) {
      perror("recvfrom");
      continue;  // Continue the loop on error
    }

    printf("Received %zd bytes from %s\n", bytes_received, inet_ntoa(from_addr.sin_addr));

    // Process the received packet (extract headers and data)
    eth_header = (struct ether_header *)packet_buffer;
    ip_header = (struct ip *)(packet_buffer + sizeof(struct ether_header));
    icmpv6_header = (struct icmpv6hdr *)(packet_buffer + sizeof(struct ether_header) + sizeof(struct ip));

    // Print information about the received packet (demonstration)
    printf("  Ethernet source: %pM, destination: %pM\n", eth_header->h_source, eth_header->h_dest);
    printf("  IP source: %s, destination: %s\n", inet_ntoa(ip_header->saddr), inet_ntoa(ip_header->daddr));
    printf("  ICMPv6 type: %d, code: %d\n", icmpv6_header->type, icmpv6_header->code);

    // Optional: handle received ICMPv6 echo replies or other packet types

    // You can add a delay between sending requests (optional)
    sleep(1);  // Delay for 1 second (example)
  }

  free(packet_buffer);
  close(sockfd);
  return 0;
}

*\
  
  


