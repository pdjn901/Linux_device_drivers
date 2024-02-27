//this is wi-fi userspace program simply calls "/dev/my_Wi-Fi_DeviceFile" 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

#define WIFI_SCAN_TIMEOUT 5000 // 5 seconds timeout for scanning

struct wifi_scan_result {
    char ssid[32];
    int signal_strength;
};

static int handle_wifi_scan(struct nl_msg *msg, void *arg) {
    struct wifi_scan_result *result = (struct wifi_scan_result *)arg;
    struct nlattr *tb[NL80211_ATTR_MAX + 1];
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
    struct nlattr *bss[NL80211_BSS_MAX + 1];
    struct nl80211_bss_info *bss_info;

    nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL);
    if (tb[NL80211_ATTR_BSS]) {
        nla_parse_nested(bss, NL80211_BSS_MAX, tb[NL80211_ATTR_BSS], NULL);
        if (bss[NL80211_BSS_BSSID] && bss[NL80211_BSS_INFORMATION_ELEMENTS]) {
            bss_info = (struct nl80211_bss_info *)nla_data(bss[NL80211_BSS_INFORMATION_ELEMENTS]);
            if (bss_info->signal_mbm) {
                snprintf(result->ssid, sizeof(result->ssid), "%s", (char *)nla_data(bss[NL80211_BSS_BSSID]));
                result->signal_strength = (int)(bss_info->signal_mbm / 100); // Convert to dBm
            }
        }
    }

    return NL_SKIP;
}

int main(void) {
    struct nl_sock *nlsock;
    struct nl_msg *msg;
    struct wifi_scan_result scan_result;
    int ret;

    // Initialize netlink socket
    nlsock = nl_socket_alloc();
    if (!nlsock) {
        fprintf(stderr, "Error: Failed to allocate netlink socket\n");
        return EXIT_FAILURE;
    }

    ret = genl_connect(nlsock);
    if (ret < 0) {
        fprintf(stderr, "Error: Failed to connect to generic netlink\n");
        nl_socket_free(nlsock);
        return EXIT_FAILURE;
    }

    ret = genl_ctrl_alloc_cache(nlsock, &nlsock);
    if (ret < 0) {
        fprintf(stderr, "Error: Failed to allocate generic netlink cache\n");
        nl_socket_free(nlsock);
        return EXIT_FAILURE;
    }

    // Allocate and prepare a scan message
    msg = nlmsg_alloc();
    if (!msg) {
        fprintf(stderr, "Error: Failed to allocate netlink message\n");
        nl_socket_free(nlsock);
        return EXIT_FAILURE;
    }

    genlmsg_put(msg, NL_AUTO_PID, NL_AUTO_SEQ, nlsock, 0, 0, NL80211_CMD_TRIGGER_SCAN, 0);
    nla_put_u32(msg, NL80211_ATTR_IFINDEX, if_nametoindex("wlan0")); // Change wlan0 to your Wi-Fi interface name

    // Send the scan message
    ret = nl_send_auto_complete(nlsock, msg);
    if (ret < 0) {
        fprintf(stderr, "Error: Failed to send netlink message\n");
        nlmsg_free(msg);
        nl_socket_free(nlsock);
        return EXIT_FAILURE;
    }

    // Wait for scan results
    ret = nl_recvmsgs(nlsock, handle_wifi_scan);
    if (ret < 0) {
        fprintf(stderr, "Error: Failed to receive netlink messages\n");
        nlmsg_free(msg);
        nl_socket_free(nlsock);
        return EXIT_FAILURE;
    }

    printf("Wi-Fi scan result:\n");
    printf("SSID: %s\n", scan_result.ssid);
    printf("Signal Strength: %d dBm\n", scan_result.signal_strength);

    nlmsg_free(msg);
    nl_socket_free(nlsock);
    return EXIT_SUCCESS;
}

/*
Note:
One common library for this purpose is the libnl library, which provides a high-level interface for working with netlink sockets and the Netlink protocol family.



*/

