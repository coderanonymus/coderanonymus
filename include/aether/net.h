#ifndef AETHER_NET_H
#define AETHER_NET_H

#include <stdint.h>
#include "aether/types.h"

typedef struct {
    uint8_t octets[4];
} ipv4_addr_t;

void net_init(void);
aether_status_t arp_resolve(ipv4_addr_t ip, uint8_t mac_out[6]);
aether_status_t icmp_ping(ipv4_addr_t ip);
aether_status_t tcp_connect(ipv4_addr_t ip, uint16_t port);
aether_status_t http_get_demo(const char *host, const char *path);

#endif
