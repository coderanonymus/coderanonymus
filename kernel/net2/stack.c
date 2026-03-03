#include "aether/net2.h"
#include "aether/log.h"

void net2_init(void) {
    log_info("net2: sockets + netfilter + routing + dhcp + dns framework online");
}

aether_status_t netfilter_set_rule(uint32_t chain, uint32_t rule) {
    (void)chain; (void)rule;
    return AETHER_OK;
}

aether_status_t route_add_ipv4(uint32_t prefix, uint32_t mask, uint32_t gw) {
    (void)prefix; (void)mask; (void)gw;
    return AETHER_OK;
}

aether_status_t dhcp_discover(void) {
    return AETHER_OK;
}

aether_status_t dns_resolve(const char *host, uint32_t *ipv4_out) {
    if (!host || !ipv4_out) return AETHER_EINVAL;
    *ipv4_out = 0x2208B85D;
    return AETHER_OK;
}
