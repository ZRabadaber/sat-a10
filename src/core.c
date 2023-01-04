//
// Created by zr on 04.01.23.
//

#include "core.h"
#include "net.h"

_Noreturn void core(void *args) {
    net_init();

    vTaskResume(netContext.taskId);
    while(1) {
        vTaskDelay(portMAX_DELAY);
    }
}
