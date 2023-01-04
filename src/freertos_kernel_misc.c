#include "FreeRTOS.h"
#include "task.h"

__attribute__ ((weak)) void vApplicationMallocFailedHook( void )
{

}

__attribute__ ((weak)) void vApplicationTickHook( void )
{

}

__attribute__ ((weak)) void vApplicationIdleHook( void )
{

}

__attribute__ ((weak)) void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{

}

/* configSUPPORT_STATIC_ALLOCATION установлен в 1, поэтому приложение должно
   предоставить реализацию vApplicationGetIdleTaskMemory(), чтобы обеспечить
   память для Idle task. */
__attribute__ ((weak)) void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* Если буферы, предоставленные для Idle task, были декларированы
      внутри этой функции, то они должны быть с атрибутом static - иначе
      они будут выделены из стека, и уничтожатся при выходе из этой функции. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Передача наружу указателя на структуру StaticTask_t, в которой
      задача Idle будет сохранять свое состояние. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Передача наружу мссива, который будет использоваться как стек */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Передача наружу размера массива, на который указывает *ppxIdleTaskStackBuffer.
      Обратите внимание, что поскольку массив должен быть типа StackType_t, то
      configMINIMAL_STACK_SIZE указывается в словах, а не в байтах. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* Оба параметра configSUPPORT_STATIC_ALLOCATION и configUSE_TIMERS установлены
   в 1, поэтому приложение должно предоставить реализацию vApplicationGetTimerTaskMemory()
   чтобы обеспечить памятью задачу Timer service. */
__attribute__ ((weak)) void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* Если буферы, предоставленные для Timer task, декларированы внутри этой
      функции, то они должны быть с атрибутом static – иначе буферы будут
      уничтожены при выходе из этой функции. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Передача наружу указателя на структуру StaticTask_t, в которой будет
      сохраняться состояние задачи Timer service. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Передача наружу мссива, который будет использоваться как стек
      задачи Timer service. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Передача наружу размера массива, на который указывает *ppxTimerTaskStackBuffer.
      Обратите внимание, что поскольку массив должен быть типа StackType_t, то
      configTIMER_TASK_STACK_DEPTH указывается в словах, а не в байтах. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
