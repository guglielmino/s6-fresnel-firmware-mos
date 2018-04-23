#pragma once

#include <string>

std::string getEthIp() {
    char eth_ip[16] = {0};
#ifdef MGOS_HAVE_ETHERNET
    struct mgos_net_ip_info ip_info;
    memset(&ip_info, 0, sizeof(ip_info));
    memset(eth_ip, 0, sizeof(eth_ip));
    if (mgos_net_get_ip_info(MGOS_NET_IF_TYPE_ETHERNET, 0, &ip_info)) {
      mgos_net_ip_to_str(&ip_info.ip, eth_ip);
    }
#endif
    return std::string(eth_ip);
}

std::string getWiFiStaIp() {
    char sta_ip[16] = {0};
#ifdef MGOS_HAVE_WIFI
    struct mgos_net_ip_info ip_info;
    memset(&ip_info, 0, sizeof(ip_info));
    memset(sta_ip, 0, sizeof(sta_ip));

    if (mgos_net_get_ip_info(MGOS_NET_IF_TYPE_WIFI, MGOS_NET_IF_WIFI_STA,
                             &ip_info)) {
      mgos_net_ip_to_str(&ip_info.ip, sta_ip);
    }
#endif
    return std::string(sta_ip);
}