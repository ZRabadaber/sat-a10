//
// Created by zraba on 14.10.2022.
//

#include "net.h"
#include "drivers/usb_rndis/rndis_driver.h"
#include <drivers/loopback/loopback_driver.h>

DhcpServerContext dhcpContext;

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

    //usb ---------------------------------------------------------------------
    interface = net_interface_get(NET_INTERFACE_USB);
    if (!interface) return -1;
    error = netSetInterfaceName(interface, "usb");
    if (error) return error;
    error = netSetDriver(interface, &rndisDriver);
    if (error) return error;
    const unsigned char _mac[6] = {
            0x20, 0x89, 0x84, 0x6A, 0x96, 0x00
    };
    netSetMacAddr(interface, (const MacAddr *) _mac);
    error = netConfigInterface(interface);
    if (error) return error;

    Ipv4Addr addr;
    //Set IPv4 host address
    ipv4StringToAddr("192.168.7.1", &addr);
    ipv4SetHostAddr(interface, addr);
    //Set subnet mask
    ipv4StringToAddr("255.255.255.0", &addr);
    ipv4SetSubnetMask(interface, addr);

    DhcpServerSettings settings;
    dhcpServerGetDefaultSettings(&settings);
    settings.interface = interface;
    //Lowest and highest IP addresses in the pool that are available
    //for dynamic address assignment
    settings.ipAddrRangeMin = IPV4_ADDR(192,168,7,2);
    settings.ipAddrRangeMax = IPV4_ADDR(192,168,7,3);
    //Subnet mask
    settings.subnetMask = IPV4_ADDR(255,255,255,0);
    //Default gateway
    settings.defaultGateway = IPV4_UNSPECIFIED_ADDR;
    dhcpServerInit(&dhcpContext, &settings);
    dhcpServerStart(&dhcpContext);

    return 0;
}
