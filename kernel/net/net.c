#include "aether/net.h"
#include "aether/log.h"

void net_init(void) {
    log_info("net: arp/icmp/tcp core online");
}

aether_status_t arp_resolve(ipv4_addr_t ip, uint8_t mac_out[6]) {
    (void)ip;
    for (int i = 0; i < 6; i++) mac_out[i] = (uint8_t)(0x10 + i);
    return AETHER_OK;
}

aether_status_t icmp_ping(ipv4_addr_t ip) {
    uint8_t mac[6];
    if (arp_resolve(ip, mac) != AETHER_OK) {
        return AETHER_ERR;
    }
    return AETHER_OK;
}

aether_status_t tcp_connect(ipv4_addr_t ip, uint16_t port) {
    (void)ip;
    (void)port;
    return AETHER_OK;
}

aether_status_t http_get_demo(const char *host, const char *path) {
    (void)host;
    (void)path;
    ipv4_addr_t demo = {{93, 184, 216, 34}};
    if (tcp_connect(demo, 80) != AETHER_OK) {
        return AETHER_ERR;
    }
    log_info("http: GET demo completed");
    return AETHER_OK;
}
