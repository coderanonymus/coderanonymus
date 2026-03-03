#ifndef AETHER_NET2_H
#define AETHER_NET2_H

#include <stdint.h>
#include "aether/types.h"

void net2_init(void);
aether_status_t netfilter_set_rule(uint32_t chain, uint32_t rule);
aether_status_t route_add_ipv4(uint32_t prefix, uint32_t mask, uint32_t gw);
aether_status_t dhcp_discover(void);
aether_status_t dns_resolve(const char *host, uint32_t *ipv4_out);

#endif
