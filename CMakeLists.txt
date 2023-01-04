cmake_minimum_required(VERSION 3.16)

include(cortex_m33)

project(sat-a10 C CXX ASM)

#redefine linker-script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32U585ZITXQ_FLASH.ld)
include(cube)

add_definitions(-DSTM32U585xx)
add_link_options(-Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)

include_directories(src)
set(SOURCES ${SOURCES} src/main.c)

include(modules)
modules_ready()

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
target_link_libraries(${PROJECT_NAME}.elf ${MODULES})
