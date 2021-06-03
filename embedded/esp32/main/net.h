#ifndef NET_H
#define NET_H

#include <stdint.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_netif_types.h"

#define TAG_ETH "net: eth"
#define TAG_IP "net: ip"

// Configure and start the ethernet interface.
esp_err_t net_eth_configure_and_start(void);

#endif
