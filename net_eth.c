//network drv eth
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>  // For ethernet header manipulation
#include <linux/ip.h>       // For IP header manipulation
#include <linux/icmpv6.h>   // For ICMPv6 header manipulation (optional)
#include <linux/slab.h>      // For kmalloc and kfree

// Replace with your desired values
#define MY_NETDEV_NAME "my_netdev"
static const unsigned char my_netdev_mac[] = {0x00, 0x0C, 0x22, 0x44, 0xAA, 0xBB};

static ssize_t my_netdev_rx(struct sk_buff *skb, struct net_device *dev) {
  struct ethhdr *eth;
  struct iphdr *iph;
  char *kernel_buffer;  // Pointer to dynamically allocated buffer

  // Check for minimum packet size (Ethernet and IP headers)
  if (skb->len < sizeof(struct ethhdr) + sizeof(struct iphdr)) {
    dev_kfree_skb(skb);
    return NET_RX_DROP;
  }

  // Allocate memory for the kernel buffer
  kernel_buffer = kmalloc(skb->len, GFP_ATOMIC);
  if (!kernel_buffer) {
    dev_kfree_skb(skb);
    return NET_RX_DROP;
  }

  // Copy packet data to kernel buffer
  memcpy(kernel_buffer, skb->data, skb->len);

  // Process the data in kernel buffer (demonstration: print headers)
  eth = (struct ethhdr *)kernel_buffer;
  iph = (struct iphdr *)(kernel_buffer + sizeof(struct ethhdr));
  printk(KERN_INFO "Received packet: source MAC: %pM, destination MAC: %pM\n",
         eth->h_source, eth->h_dest);
  printk(KERN_INFO "  Source IP: %pI4, Destination IP: %pI4\n",
         &iph->saddr, &iph->daddr);

  // Optional processing based on packet type (e.g., ICMPv6 echo reply handling)
  if (iph->protocol == IPPROTO_ICMPV6) {
    struct icmpv6hdr *icmpv6 = (struct icmpv6hdr *)(kernel_buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    if (icmpv6->type == ICMPV6_ECHO_REPLY) {
      // Handle ICMPv6 echo reply (demonstration: print message)
      printk(KERN_INFO "  ICMPv6 echo reply received!\n");
    }
  }

  // Free the dynamically allocated buffer
  kfree(kernel_buffer);

  // Free the original skb
  dev_kfree_skb(skb);

  return NET_RX_SUCCESS;
}

static netdev_tx_t my_netdev_tx(struct sk_buff *skb, struct net_device *dev) {
  struct ethhdr *eth;
  struct iphdr *iph;
  int headroom;  // Space for new headers (optional)

  // Optional: prepend space for new headers (e.g., VLAN)
  headroom = netdev_gadgets_validate_headers(skb, dev, MY_NETDEV_NAME);
  if (headroom < 0) {
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;  // Indicate successful processing (no transmission)
  }

  // Access and potentially modify ethernet and IP headers
  eth = (struct ethhdr *)skb->data;
  iph = (struct iphdr *)(skb->data + sizeof(struct ethhdr));

  // Modify destination MAC address (demonstration)
  memcpy(eth->h_dest, my_netdev_mac, ETH_ALEN);

  // Optional: modify IP header fields (e.g., TTL decrement)
  iph->ttl--;  // Decrement Time to Live for forwarding (example)

  // Simulate packet transmission (replace with your actual transmission logic)
  printk(KERN_INFO "my_netdev: Transmitting packet...\n");

  // Free the skb (assuming transmission is simulated

}



static const struct net_device_ops my_netdev_ops = {
  .ndo_rx = my_netdev_rx,
  .ndo_tx = my_netdev_tx,
  // ... other net device operations (optional)
};

static struct net_device my_netdev = {
  .netdev_ops = &my_netdev_ops,
  .name = MY_NETDEV_NAME,
  .addr = my_netdev_mac,
  // ... other net device configuration (e.g., MTU, flags)
};

int __init my_netdev_init(void) {
  // Set additional device configuration (optional)
  my_netdev.mtu = 1500;  // Set Maximum Transmission Unit (MTU)

  // Register the network device
  if (netdev_register(&my_netdev)) {
    printk(KERN_ERR "Failed to register my_netdev\n");
    return -1;
  }

  printk(KERN_INFO "my_netdev driver initialized\n");
  return 0;
}

void __exit my_netdev_exit(void) {
  // Unregister the network device
  netdev_unregister(&my_netdev);
}

module_init(my_netdev_init);
module_exit(my_netdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple network driver with dynamic buffer allocation");
