# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

project(hello_world)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/_deploy/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/_deploy/${CMAKE_BUILD_TYPE})

set(MCUX_SDK_PROJECT_NAME hello_world.elf)

include(${PROJECT_SOURCE_DIR}/./flags.cmake)

include(${PROJECT_SOURCE_DIR}/./config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME}
"${PROJECT_SOURCE_DIR}/board/board.c"
"${PROJECT_SOURCE_DIR}/board/board.h"
"${PROJECT_SOURCE_DIR}/board/clock_config.c"
"${PROJECT_SOURCE_DIR}/board/clock_config.h"
"${PROJECT_SOURCE_DIR}/board/pin_mux.c"
"${PROJECT_SOURCE_DIR}/board/pin_mux.h"
"${PROJECT_SOURCE_DIR}/source/hello_world.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
${PROJECT_SOURCE_DIR}/board
)

set(CMAKE_MODULE_PATH
${PROJECT_SOURCE_DIR}/.
${PROJECT_SOURCE_DIR}/CMSIS
${PROJECT_SOURCE_DIR}/component/lists
${PROJECT_SOURCE_DIR}/component/uart
${PROJECT_SOURCE_DIR}/device
${PROJECT_SOURCE_DIR}/drivers
${PROJECT_SOURCE_DIR}/startup
${PROJECT_SOURCE_DIR}/utilities
)

# include modules
include(utility_debug_console_lite_MK64F12)

include(utility_assert_lite_MK64F12)

include(driver_gpio_MK64F12)

include(driver_flash_MK64F12)

include(driver_clock_MK64F12)

include(driver_port_MK64F12)

include(driver_uart_MK64F12)

include(driver_smc_MK64F12)

include(device_MK64F12_CMSIS_MK64F12)

include(component_uart_adapter_MK64F12)

include(driver_common_MK64F12)

include(component_lists_MK64F12)

include(device_MK64F12_startup_MK64F12)

include(CMSIS_Include_core_cm_MK64F12)

include(utilities_misc_utilities_MK64F12)

include(device_MK64F12_system_MK64F12)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


