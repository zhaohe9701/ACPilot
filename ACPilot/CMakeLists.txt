cmake_minimum_required(VERSION 3.24)

set(ESP32 TRUE)
add_compile_definitions(C_ESP32)

set(EXTRA_COMPONENT_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/Common
        ${CMAKE_CURRENT_SOURCE_DIR}/Component
        ${CMAKE_CURRENT_SOURCE_DIR}/Osal
        ${CMAKE_CURRENT_SOURCE_DIR}/Utils
        ${CMAKE_CURRENT_SOURCE_DIR}/Interface
        ${CMAKE_CURRENT_SOURCE_DIR}/Framework
        ${CMAKE_CURRENT_SOURCE_DIR}/Application
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers
        ${CMAKE_CURRENT_SOURCE_DIR}/Sdk
        ${CMAKE_CURRENT_SOURCE_DIR}/Service
        )

if (STM32)
        include(${CMAKE_CURRENT_SOURCE_DIR}/sdk/project.cmake)
endif()

if (ESP32)
        include($ENV{IDF_PATH}/tools/cmake/project.cmake)
endif()

project(ACPilot C CXX ASM)
