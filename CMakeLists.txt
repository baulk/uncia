# Author: Force Charlie Copyright (C) 2022. Force Charlie. All Rights Reserved.
cmake_minimum_required(VERSION 3.20)
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
  set(CMAKE_MSVC_RUNTIME_LIBRARY_DEFAULT "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  add_compile_options("-D_CRT_SECURE_NO_WARNINGS=1")
  add_compile_options("-utf-8")
  add_compile_options("-permissive-")
  add_compile_options("-Zc:__cplusplus")
  add_compile_options("-DUNICODE=1")
  add_compile_options("-D_UNICODE=1")
  add_compile_options("-W3")
  add_compile_options("-wd26812")
  set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -c65001")
else()
  add_compile_options("-DUNICODE=1")
  add_compile_options("-D_UNICODE=1")
  add_compile_options("-g")
  add_compile_options("-Wall")
  add_compile_options("-Wextra")
  if(USE_SANITIZE)
    add_compile_options("-fsanitize=address")
    add_compile_options("-fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address ")
  endif()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
# Gen version
include(VersionFromVCS)
get_source_info(${CMAKE_CURRENT_SOURCE_DIR} BAULKTAR_REVISION BAULKTAR_REMOTE_URL BAULKTAR_REFNAME)
string(TIMESTAMP BAULKTAR_COPYRIGHT_YEAR "%Y")

# BAULKTAR_REFNAME maybe defined by ci
if("${BAULKTAR_REFNAME}" STREQUAL "none")
  if(DEFINED ENV{GITHUB_REF})
    set(BAULKTAR_REFNAME $ENV{GITHUB_REF_NAME})
  endif()
endif()

message(STATUS "BAULKTAR_REFNAME: ${BAULKTAR_REFNAME}")

if("${BAULKTAR_REFNAME}" MATCHES "refs/tags/*")
  string(TIMESTAMP BAULKTAR_BUILD_TIME "%Y-%m-%dT%H:%M:%SZ")
else()
  set(BAULKTAR_BUILD_TIME "none")
endif()

if(DEFINED ENV{GITHUB_RUN_NUMBER})
  set(BAULKTAR_VERSION_BUILD $ENV{GITHUB_RUN_NUMBER})
  message(STATUS "baulktar detect GITHUB_RUN_NUMBER: $ENV{GITHUB_RUN_NUMBER}")
else()
  set(BAULKTAR_VERSION_BUILD 1)
endif()

set(BAULKTAR_VERSION_MAJOR 2)
set(BAULKTAR_VERSION_MINOR 0)
set(BAULKTAR_VERSION_PATCH 0)
set(PACKAGE_VERSION "${BAULKTAR_VERSION_MAJOR}.${BAULKTAR_VERSION_MINOR}.${BAULKTAR_VERSION_PATCH}")

string(TOLOWER "${CMAKE_C_COMPILER_ARCHITECTURE_ID}" BAULKTAR_ARCH_NAME)
set(CPACK_SYSTEM_NAME "win-${BAULKTAR_ARCH_NAME}")
set(CPACK_PACKAGE_NAME "BaulkTar")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "BaulkTar")
set(CPACK_PACKAGE_VERSION_MAJOR ${BAULKTAR_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${BAULKTAR_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${BAULKTAR_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_PACKAGE_VENDOR "BaulkTar")
set(CPACK_PACKAGE_CONTACT "Baulk contributors")

include(CPack)
# Generate version code
configure_file(${CMAKE_SOURCE_DIR}/include/revision.h.cmake ${CMAKE_BINARY_DIR}/include/revision.h)

set(CMAKE_CXX_STANDARD 20)
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

add_definitions(-D_UNICODE -DUNICODE -DLIBARCHIVE_STATIC=1)

include_directories("${CMAKE_BINARY_DIR}/include" "./include" "./vendor/bela/include" "./lib/libarchive")

add_subdirectory(lib)
add_subdirectory(vendor/bela)
add_subdirectory(tools/bsdtar)
add_subdirectory(tools/uncia)
