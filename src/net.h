//
// Created by zraba on 14.10.2022.
//

#ifndef SATELLIT_A10_SRC_NET_H_
#define SATELLIT_A10_SRC_NET_H_

#include <core/net.h>

typedef enum {
  NET_INTERFACE_LO = 0,
  NET_INTERFACE_USB,
  NET_INTERFACE_GSM,
  NET_INTERFACE_ETH0,
  NET_INTERFACE_ETH1
} NET_INTERFACE_IDX;

int net_init(void);
NetInterface *net_interface_get(NET_INTERFACE_IDX idx);

#endif //SATELLIT_A10_SRC_NET_H_
