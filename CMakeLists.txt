cmake_minimum_required(VERSION 3.16)

include(cortex_m33)

project(sat-a10 C CXX ASM)

#redefine linker-script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32U585ZITXQ_FLASH.ld)
include(cube)

add_definitions(-DSTM32U585xx)
add_link_options(-Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)

include_directories(src)
set(SOURCES ${SOURCES} src/main.c src/freertos_kernel_misc.c src/core.c)

include(modules)
#FreeRTOS ----------------------------------------------------------------------
set(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/src" CACHE STRING "")
set(FREERTOS_PORT "GCC_ARM_CM33_NTZ_NONSECURE" CACHE STRING "")
set(FREERTOS_HEAP "5" CACHE STRING "")
module(freertos_kernel GIT_REPOSITORY git@github.com:FreeRTOS/FreeRTOS-Kernel.git GIT_TAG 6d65558ba01141d7c50ff6f93a4054cc5f16940e)
modules_ready()

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
target_link_libraries(${PROJECT_NAME}.elf ${MODULES})
