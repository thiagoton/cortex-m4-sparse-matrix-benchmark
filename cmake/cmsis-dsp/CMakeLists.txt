cmake_minimum_required(VERSION 3.14)

project(CMSISDSPStandalone C)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(BUILD_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMSISDSP_INSTALL OFF)

include("${PROJECT_ROOT}/cmake/flags.cmake")

add_subdirectory(
    "${PROJECT_ROOT}/src/CMSIS/DSP/Source"
    "${CMAKE_BINARY_DIR}/cmsis-dsp"
)

target_include_directories(CMSISDSP PUBLIC
    "${PROJECT_ROOT}/src/CMSIS"
)

set_target_properties(CMSISDSP PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
