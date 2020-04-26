# Author: Force Charlie Copyright (C) 2020. Force Charlie. All Rights Reserved.
cmake_minimum_required(VERSION 3.15)
project(BaulkTar)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
  message(
    FATAL_ERROR
      "In-source builds are not allowed.
CMake would overwrite the makefiles distributed with Baulk.
Please create a directory and run cmake from there, passing the path
to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
Please delete them.")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
option(BUILD_TEST "build test" OFF)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY_DEFAULT "")
  # FIXME: This block should go away after a transition period.
  if(NOT CMAKE_VERSION VERSION_LESS 3.15)
    # Filter out MSVC runtime library flags that may have come from the cache of an existing build tree or from scripts.
    foreach(l C CXX)
      foreach(c DEBUG MINSIZEREL RELEASE RELWITHDEBINFO)
        string(REGEX REPLACE "[-/]M[DT]d?( |$)" "" "CMAKE_${l}_FLAGS_${c}" "${CMAKE_${l}_FLAGS_${c}}")
      endforeach()
    endforeach()
  endif()
  # http://stackoverflow.com/a/32128977/486990
  add_compile_options(
    "$<$<CONFIG:Debug>:/MTd>" # -MTd
    "$<$<CONFIG:RelWithDebInfo>:/MT>" # -MT
    "$<$<CONFIG:Release>:/MT>" # * MT
    "$<$<CONFIG:MinSizeRel>:/MT>")
  add_compile_options("-D_CRT_SECURE_NO_WARNINGS=1")
  add_compile_options("-utf-8")
  add_compile_options("-permissive-")
  add_compile_options("-Zc:__cplusplus")
  add_compile_options("-DUNICODE=1")
  add_compile_options("-D_UNICODE=1")
  set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -c65001")
else()
  add_compile_options("-g")
  add_compile_options("-Wall")
  add_compile_options("-Wextra")
  if(USE_SANITIZE)
    add_compile_options("-fsanitize=address")
    add_compile_options("-fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address ")
  endif()
endif()

set(BAULKTAR_VERSION_MAJOR 1)
set(BAULKTAR_VERSION_MINOR 0)
set(BAULKTAR_VERSION_PATCH 0)
set(PACKAGE_VERSION "${BAULKTAR_VERSION_MAJOR}.${BAULKTAR_VERSION_MINOR}.${BAULKTAR_VERSION_PATCH}")

set(CPACK_PACKAGE_NAME "BaulkTar")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "BaulkTar")
set(CPACK_PACKAGE_VERSION_MAJOR ${BAULKTAR_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${BAULKTAR_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${BAULKTAR_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
if("${CMAKE_C_COMPILER_ARCHITECTURE_ID}" STREQUAL "ARM64" OR "${CMAKE_C_COMPILER_ARCHITECTURE_ID}" STREQUAL "ARM")
  set(CPACK_SYSTEM_NAME "windows-${CMAKE_C_COMPILER_ARCHITECTURE_ID}")
endif()
set(CPACK_PACKAGE_VENDOR "BaulkTar")
set(CPACK_PACKAGE_CONTACT "BaulkTar contributors")

include(CPack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(BAULKTAR_ENABLE_LTO OFF)
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT error)
if(lto_supported)
  message(STATUS "IPO/LTO supported")
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BAULKTAR_ENABLE_LTO ON)
    set(BELA_ENABLE_LTO ON)
    message(STATUS "IPO/LTO enabled")
  endif()
endif()

add_definitions(-D_UNICODE -DUNICODE)

add_subdirectory(lib)
add_subdirectory(vendor/bela)
add_subdirectory(tools/baulktar)
add_subdirectory(tools/bsdtar)