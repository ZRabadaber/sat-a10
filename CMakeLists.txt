cmake_minimum_required(VERSION 3.16)

include(cortex_m33)

project(sat-a10 C CXX ASM)

#redefine linker-script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32U585ZITXQ_FLASH.ld)
include(cube)

add_definitions(-DSTM32U585xx)
add_link_options(-Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)

include_directories(src)
set(SOURCES ${SOURCES} src/main.c src/freertos_kernel_misc.c src/core.c src/net.c src/usbd_conf.c)

include(modules)

#FreeRTOS ----------------------------------------------------------------------
#set(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/src" CACHE STRING "")
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/src)
target_compile_definitions(freertos_config
        INTERFACE
        projCOVERAGE_TEST=0)
set(FREERTOS_PORT "GCC_ARM_CM33_NTZ_NONSECURE" CACHE STRING "")
set(FREERTOS_HEAP "5" CACHE STRING "")
module(freertos_kernel GIT_REPOSITORY git@github.com:FreeRTOS/FreeRTOS-Kernel.git GIT_TAG 6d65558ba01141d7c50ff6f93a4054cc5f16940e)

#Cyclone -----------------------------------------------------------------------
add_definitions(-DGPL_LICENSE_TERMS_ACCEPTED)
add_definitions(-D__USE_W32_SOCKETS)

module(CycloneCommon GIT_REPOSITORY git@github.com:Oryx-Embedded/Common.git GIT_TAG ee1e3c0c0d0747cddbb65ca4d0fcdf0f5a9c89fd)
module_no_build(CycloneCommon)
include_directories(${CycloneCommon_SOURCE_DIR})
set(SOURCES ${SOURCES} ${CycloneCommon_SOURCE_DIR}/cpu_endian.c
        ${CycloneCommon_SOURCE_DIR}/date_time.c
        ${CycloneCommon_SOURCE_DIR}/debug.c
        ${CycloneCommon_SOURCE_DIR}/os_port_freertos.c
        ${CycloneCommon_SOURCE_DIR}/path.c
        ${CycloneCommon_SOURCE_DIR}/str.c)

module(CycloneTCP GIT_REPOSITORY git@github.com:Oryx-Embedded/CycloneTCP.git GIT_TAG e082443a79516617434a5db5b27d93eef5490102)
module_no_build(CycloneTCP)
include_directories(${CycloneTCP_SOURCE_DIR})
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${CycloneTCP_SOURCE_DIR}/core/*.c)
set(SOURCES ${SOURCES} ${SRC})
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${CycloneTCP_SOURCE_DIR}/ipv4/*.c)
set(SOURCES ${SOURCES} ${SRC})
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${CycloneTCP_SOURCE_DIR}/dns/*.c)
set(SOURCES ${SOURCES} ${SRC})
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${CycloneTCP_SOURCE_DIR}/netbios/*.c)
set(SOURCES ${SOURCES} ${SRC})
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${CycloneTCP_SOURCE_DIR}/igmp/*.c)
set(SOURCES ${SOURCES} ${SRC})
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${CycloneTCP_SOURCE_DIR}/dhcp/*.c)
set(SOURCES ${SOURCES} ${SRC})
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${CycloneTCP_SOURCE_DIR}/drivers/loopback/*.c)
set(SOURCES ${SOURCES} ${SRC})
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${CycloneTCP_SOURCE_DIR}/drivers/usb_rndis/*.c)
set(SOURCES ${SOURCES} ${SRC})

#USB Device --------------------------------------------------------------------
module(stm32_usb_device_lib GIT_REPOSITORY git@github.com:STMicroelectronics/stm32_mw_usb_device.git GIT_TAG 2022e75b01a499b17acd17d28691b1ed5bbef2dc)
module_no_build(stm32_usb_device_lib)
include_directories(${stm32_usb_device_lib_SOURCE_DIR}/Core/Inc)
file(GLOB_RECURSE SRC LIST_DIRECTORIES false ${stm32_usb_device_lib_SOURCE_DIR}/Core/Src/*.c)
list(FILTER SRC EXCLUDE REGEX "^.*template.c$")
set(SOURCES ${SOURCES} ${SRC})

modules_ready()

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
target_link_libraries(${PROJECT_NAME}.elf ${MODULES})
