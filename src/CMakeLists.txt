# CROSS COMPILER SETTING
add_executable(${MCUX_SDK_PROJECT_NAME}
"${CMAKE_CURRENT_SOURCE_DIR}/board/board.c"
"${CMAKE_CURRENT_SOURCE_DIR}/board/board.h"
"${CMAKE_CURRENT_SOURCE_DIR}/board/clock_config.c"
"${CMAKE_CURRENT_SOURCE_DIR}/board/clock_config.h"
"${CMAKE_CURRENT_SOURCE_DIR}/board/pin_mux.c"
"${CMAKE_CURRENT_SOURCE_DIR}/board/pin_mux.h"
"${CMAKE_CURRENT_SOURCE_DIR}/test_suites/naive_mat_mult.c"
"${CMAKE_CURRENT_SOURCE_DIR}/test_suites/test_matrix_f32.c"
"${CMAKE_CURRENT_SOURCE_DIR}/test_suites/test_matrix_q15.c"
"${CMAKE_CURRENT_SOURCE_DIR}/test_suites/test_common.c"
"${CMAKE_CURRENT_SOURCE_DIR}/test_suites/sparse_matrix.c"
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

include(driver_rnga_MK64F12)

include(ExternalProject)

set(CMSISDSP_BUILD_DIR "${PROJECT_SOURCE_DIR}/_build/cmsis-dsp/${CMAKE_BUILD_TYPE}")
set(CMSISDSP_ARCHIVE "${CMSISDSP_BUILD_DIR}/lib/libCMSISDSP.a")

ExternalProject_Add(cmsisdsp_external
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/cmsis-dsp"
    BINARY_DIR "${CMSISDSP_BUILD_DIR}"
    CMAKE_ARGS
        "-DCMAKE_TOOLCHAIN_FILE=${PROJECT_SOURCE_DIR}/cmake/armgcc.cmake"
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    BUILD_ALWAYS TRUE
    BUILD_BYPRODUCTS "${CMSISDSP_ARCHIVE}"
    INSTALL_COMMAND ""
)

add_library(CMSISDSP STATIC IMPORTED GLOBAL)
set_target_properties(CMSISDSP PROPERTIES
    IMPORTED_LOCATION "${CMSISDSP_ARCHIVE}"
    INTERFACE_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/DSP/Include;${CMAKE_CURRENT_SOURCE_DIR}/CMSIS"
)
add_dependencies(${MCUX_SDK_PROJECT_NAME} cmsisdsp_external)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE CMSISDSP)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)
