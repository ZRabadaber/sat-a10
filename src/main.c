#include <main.h>
#include <icache.h>
#include <gpio.h>

#include <FreeRTOS.h>
#include <task.h>

#include "core.h"

extern void SystemClock_Config(void);
__weak void PeriphCommonClock_Config(void) {

}

uint8_t ucHeap[configTOTAL_HEAP_SIZE];
HeapRegion_t xHeapRegions[] = {
        {&ucHeap[0], sizeof(ucHeap)},
        {NULL,       0}
};

int main(void) {
    HAL_Init();
    SystemClock_Config();
    PeriphCommonClock_Config();

    HAL_PWREx_DisableUCPDDeadBattery();
    if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK) {
        Error_Handler();
    }

    MX_ICACHE_Init();
    MX_GPIO_Init();

    //start application
    vPortDefineHeapRegions(xHeapRegions);
    xTaskCreate(core,
                "core",
                1024,
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    vTaskStartScheduler();
}
