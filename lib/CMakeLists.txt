# btar archives

# brotli
file(
  GLOB
  BROTLI_SOURCES
  brotli/common/*.c
  brotli/dec/*.c
  brotli/enc/*.c)
add_library(brotli STATIC ${BROTLI_SOURCES})
target_include_directories(brotli PRIVATE brotli/include)

# bzip2
file(GLOB BZIP2_SOURCES bzip2/*.c)
add_library(bzip2 STATIC ${BZIP2_SOURCES})

add_subdirectory(ced)
add_subdirectory(xz)

add_library(lz4 STATIC lz4/lz4.c lz4/lz4frame.c lz4/lz4hc.c lz4/xxhash.c)

# zlib
set(ZLIB_INC)
if("${CMAKE_C_COMPILER_ARCHITECTURE_ID}" STREQUAL "x86" OR "${CMAKE_C_COMPILER_ARCHITECTURE_ID}" STREQUAL "x64")
  set(ZLIB_SOURCES
      zlib-intel/adler32.c
      zlib-intel/compress.c
      zlib-intel/crc_folding.c
      zlib-intel/crc32.c
      zlib-intel/deflate_medium.c
      zlib-intel/deflate_quick.c
      zlib-intel/deflate.c
      zlib-intel/inffast.c
      zlib-intel/inflate.c
      zlib-intel/inftrees.c
      zlib-intel/match.c
      zlib-intel/slide_sse.c
      zlib-intel/trees.c
      zlib-intel/x86.c
      zlib-intel/zutil.c)
  set(ZLIB_INC zlib-intel)
  list(APPEND ARCHIVE_INC zlib-intel)
else()
  set(ZLIB_SOURCES
      zlib/adler32.c
      zlib/compress.c
      zlib/crc32.c
      zlib/deflate.c
      zlib/inffast.c
      zlib/inflate.c
      zlib/inftrees.c
      zlib/trees.c
      zlib/zutil.c)
  set(ZLIB_INC zlib)
  list(APPEND ARCHIVE_INC zlib)
endif()

add_library(zlib STATIC ${ZLIB_SOURCES})

# zstd lib
file(
  GLOB
  ZSTD_SOURCES
  zstd/common/*.c
  zstd/compress/*.c
  zstd/decompress/*.c
  zstd/dictBuilder/*.c)
add_library(zstd STATIC ${ZSTD_SOURCES})
target_compile_definitions(zstd PRIVATE XXH_PRIVATE_API ZSTD_MULTITHREAD ZSTD_DISABLE_ASM)
target_include_directories(zstd PRIVATE zstd zstd/common)

# libarchive
file(GLOB LIBARCHIVE_SOURCES libarchive/*.c libarchive/*.h)
add_library(archive STATIC ${LIBARCHIVE_SOURCES})
target_include_directories(
  archive
  PRIVATE bzip2
          zlib
          xz/src/liblzma/api
          lz4
          zstd
          brotli/include)
target_compile_definitions(archive PRIVATE LZMA_API_STATIC=1 HAVE_CONFIG_H=1)
target_link_libraries(
  archive
  brotli
  bzip2
  liblzma
  lz4
  zlib
  zstd)

# re2 lib
file(GLOB RE2_SOURCES re2/re2/*.cc)
add_library(re2 STATIC ${RE2_SOURCES})
target_compile_definitions(
  re2
  PRIVATE UNICODE=1
          _UNICODE=1
          STRICT
          NOMINMAX
          _CRT_SECURE_NO_WARNINGS
          _SCL_SECURE_NO_WARNINGS)
target_include_directories(re2 PRIVATE re2)
target_compile_options(
  re2
  PRIVATE -wd4100
          -wd4201
          -wd4456
          -wd4457
          -wd4702
          -wd4815)

file(GLOB UNCIA_ARCHIVE_SOURCES *.cc)

add_library(uncia.archive STATIC ${UNCIA_ARCHIVE_SOURCES})

target_include_directories(
  uncia.archive
  PRIVATE bzip2
          zlib
          xz/src/liblzma/api
          zstd
          brotli/include
          ced)
target_compile_options(uncia.archive PRIVATE ${LZMA_DEF})
target_link_libraries(
  uncia.archive
  belawin
  archive
  ced
  liblzma
  advapi32)
