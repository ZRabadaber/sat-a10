//
// Created by zraba on 14.10.2022.
//

#include "net.h"
//#include "drivers/usb_rndis/rndis_driver.h"
#include <drivers/loopback/loopback_driver.h>

NetInterface *net_interface_get(NET_INTERFACE_IDX idx) {
    if (idx >= NET_INTERFACE_COUNT) return 0;
    return &netInterface[idx];
}

int net_init(void) {
    error_t error = netInit();
    if (error) return error;
    vTaskSuspend(netContext.taskId);

    //loopback ----------------------------------------------------------------
    NetInterface *interface = net_interface_get(NET_INTERFACE_LO);
    if (!interface) return -1;
    error = netSetInterfaceName(interface, "lo");
    if (error) return error;
    error = netSetDriver(interface, &loopbackDriver);
    if (error) return error;
    error = netConfigInterface(interface);
    if (error) return error;

//    //usb ---------------------------------------------------------------------
//    interface = net_interface_get(NET_INTERFACE_USB);
//    if (!interface) return -1;
//    error = netSetInterfaceName(interface, "usb");
//    if (error) return error;
//    error = netSetDriver(interface, &rndisDriver);
//    if (error) return error;
//    const unsigned char eth0_mac[6] = {
//            0x00,0x02,0x02,0x03,0x00,0x00
//    };
//    netSetMacAddr(interface, (const MacAddr *)eth0_mac);
//    error = netConfigInterface(interface);
//    if (error) return error;
//
//    //usb ip-address ----------------------------------------------------------
//    Ipv4Addr addr;
//    //Set IPv4 host address
//    ipv4StringToAddr("192.168.7.1", &addr);
//    ipv4SetHostAddr(interface, addr);
//    //Set subnet mask
//    ipv4StringToAddr("255.255.255.0", &addr);
//    ipv4SetSubnetMask(interface, addr);
//    //Set default gateway
//    ipv4StringToAddr("0.0.0.0", &addr);
//    ipv4SetDefaultGateway(interface, addr);
//    //Set primary and secondary DNS servers
//    ipv4StringToAddr("0.0.0.0", &addr);
//    ipv4SetDnsServer(interface, 0, addr);
//    ipv4StringToAddr("0.0.0.0", &addr);
//    ipv4SetDnsServer(interface, 1, addr);

    return 0;
}
