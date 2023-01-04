#include <main.h>
#include <icache.h>
#include <gpio.h>

extern void SystemClock_Config(void);

void main(void) {
  HAL_Init();
  SystemClock_Config();

  HAL_PWREx_DisableUCPDDeadBattery();
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK) {
    Error_Handler();
  }

  MX_ICACHE_Init();
  MX_GPIO_Init();

  while (1) {}
}
