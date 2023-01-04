//
// Created by zr on 04.01.23.
//

#include "core.h"
#include "net.h"

#include <usbd_core.h>
#include <drivers/usb_rndis/usbd_desc.h>
#include <drivers/usb_rndis/usbd_rndis.h>
USBD_HandleTypeDef USBD_Device;

_Noreturn void core(void *args) {
    net_init();

    USBD_StatusTypeDef status = USBD_Init(&USBD_Device, &usbdRndisDescriptors, 0);
    status = USBD_RegisterClass(&USBD_Device, &usbdRndisClass);
    USBD_Start(&USBD_Device);

    vTaskResume(netContext.taskId);
    while(1) {
        vTaskDelay(portMAX_DELAY);
    }
}
