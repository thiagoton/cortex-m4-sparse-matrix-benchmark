# CROSS COMPILER SETTING
add_executable(${MCUX_SDK_PROJECT_NAME}
"${CMAKE_CURRENT_SOURCE_DIR}/board/board.c"
"${CMAKE_CURRENT_SOURCE_DIR}/board/board.h"
"${CMAKE_CURRENT_SOURCE_DIR}/board/clock_config.c"
"${CMAKE_CURRENT_SOURCE_DIR}/board/clock_config.h"
"${CMAKE_CURRENT_SOURCE_DIR}/board/pin_mux.c"
"${CMAKE_CURRENT_SOURCE_DIR}/board/pin_mux.h"
"${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/board
)

set(CMAKE_MODULE_PATH
${CMAKE_CURRENT_SOURCE_DIR}/.
${CMAKE_CURRENT_SOURCE_DIR}/CMSIS
${CMAKE_CURRENT_SOURCE_DIR}/component/lists
${CMAKE_CURRENT_SOURCE_DIR}/component/uart
${CMAKE_CURRENT_SOURCE_DIR}/device
${CMAKE_CURRENT_SOURCE_DIR}/drivers
${CMAKE_CURRENT_SOURCE_DIR}/startup
${CMAKE_CURRENT_SOURCE_DIR}/utilities
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


