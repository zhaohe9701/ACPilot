# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/Espressif/frameworks/esp-idf-v5.1.1/components/bootloader/subproject"
  "F:/ACPilot_ESP/ACPilot/cmake-build-debug/bootloader"
  "F:/ACPilot_ESP/ACPilot/cmake-build-debug/bootloader-prefix"
  "F:/ACPilot_ESP/ACPilot/cmake-build-debug/bootloader-prefix/tmp"
  "F:/ACPilot_ESP/ACPilot/cmake-build-debug/bootloader-prefix/src/bootloader-stamp"
  "F:/ACPilot_ESP/ACPilot/cmake-build-debug/bootloader-prefix/src"
  "F:/ACPilot_ESP/ACPilot/cmake-build-debug/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/ACPilot_ESP/ACPilot/cmake-build-debug/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/ACPilot_ESP/ACPilot/cmake-build-debug/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
