# btar archives

# Brotli
set(BROTLI_SOURCES
    brotli/common/dictionary.c
    brotli/common/transform.c
    brotli/dec/bit_reader.c
    brotli/dec/decode.c
    brotli/dec/huffman.c
    brotli/dec/state.c
    brotli/enc/backward_references.c
    brotli/enc/backward_references_hq.c
    brotli/enc/bit_cost.c
    brotli/enc/block_splitter.c
    brotli/enc/brotli_bit_stream.c
    brotli/enc/cluster.c
    brotli/enc/compress_fragment.c
    brotli/enc/compress_fragment_two_pass.c
    brotli/enc/dictionary_hash.c
    brotli/enc/encode.c
    brotli/enc/encoder_dict.c
    brotli/enc/entropy_encode.c
    brotli/enc/histogram.c
    brotli/enc/literal_cost.c
    brotli/enc/memory.c
    brotli/enc/metablock.c
    brotli/enc/static_dict.c
    brotli/enc/utf8_util.c)

add_library(brotli STATIC ${BROTLI_SOURCES})
target_include_directories(brotli PRIVATE brotli/include)

set(ARCHIVE_INC)
list(APPEND ARCHIVE_INC brotli/include)

add_library(
  bzip2 STATIC
  bzip2/blocksort.c
  bzip2/bzlib.c
  bzip2/compress.c
  bzip2/crctable.c
  bzip2/decompress.c
  bzip2/huffman.c
  bzip2/randtable.c)

list(APPEND ARCHIVE_INC bzip2)
# liblzma
add_library(
  lzma STATIC
  liblzma/common/tuklib_cpucores.c
  liblzma/common/tuklib_physmem.c
  liblzma/check/check.c
  liblzma/check/crc32_fast.c
  liblzma/check/crc32_table.c
  liblzma/check/crc64_fast.c
  liblzma/check/crc64_table.c
  liblzma/check/sha256.c
  liblzma/common/alone_decoder.c
  liblzma/common/alone_encoder.c
  liblzma/common/auto_decoder.c
  liblzma/common/block_buffer_decoder.c
  liblzma/common/block_buffer_encoder.c
  liblzma/common/block_decoder.c
  liblzma/common/block_encoder.c
  liblzma/common/block_header_decoder.c
  liblzma/common/block_header_encoder.c
  liblzma/common/block_util.c
  liblzma/common/common.c
  liblzma/common/easy_buffer_encoder.c
  liblzma/common/easy_decoder_memusage.c
  liblzma/common/easy_encoder.c
  liblzma/common/easy_encoder_memusage.c
  liblzma/common/easy_preset.c
  liblzma/common/filter_buffer_decoder.c
  liblzma/common/filter_buffer_encoder.c
  liblzma/common/filter_common.c
  liblzma/common/filter_decoder.c
  liblzma/common/filter_encoder.c
  liblzma/common/filter_flags_decoder.c
  liblzma/common/filter_flags_encoder.c
  liblzma/common/hardware_cputhreads.c
  liblzma/common/hardware_physmem.c
  liblzma/common/index.c
  liblzma/common/index_decoder.c
  liblzma/common/index_encoder.c
  liblzma/common/index_hash.c
  liblzma/common/outqueue.c
  liblzma/common/stream_buffer_decoder.c
  liblzma/common/stream_buffer_encoder.c
  liblzma/common/stream_decoder.c
  liblzma/common/stream_encoder.c
  liblzma/common/stream_encoder_mt.c
  liblzma/common/stream_flags_common.c
  liblzma/common/stream_flags_decoder.c
  liblzma/common/stream_flags_encoder.c
  liblzma/common/vli_decoder.c
  liblzma/common/vli_encoder.c
  liblzma/common/vli_size.c
  liblzma/delta/delta_common.c
  liblzma/delta/delta_decoder.c
  liblzma/delta/delta_encoder.c
  liblzma/lzma/fastpos_table.c
  liblzma/lzma/lzma2_decoder.c
  liblzma/lzma/lzma2_encoder.c
  liblzma/lzma/lzma_decoder.c
  liblzma/lzma/lzma_encoder.c
  liblzma/lzma/lzma_encoder_optimum_fast.c
  liblzma/lzma/lzma_encoder_optimum_normal.c
  liblzma/lzma/lzma_encoder_presets.c
  liblzma/lz/lz_decoder.c
  liblzma/lz/lz_encoder.c
  liblzma/lz/lz_encoder_mf.c
  liblzma/rangecoder/price_table.c
  liblzma/simple/arm.c
  liblzma/simple/armthumb.c
  liblzma/simple/ia64.c
  liblzma/simple/powerpc.c
  liblzma/simple/simple_coder.c
  liblzma/simple/simple_decoder.c
  liblzma/simple/simple_encoder.c
  liblzma/simple/sparc.c
  liblzma/simple/x86.c)

set(LZMA_INC)
list(APPEND LZMA_INC liblzma)
list(APPEND LZMA_INC liblzma/api)
list(APPEND LZMA_INC liblzma/check)
list(APPEND LZMA_INC liblzma/common)
list(APPEND LZMA_INC liblzma/delta)
list(APPEND LZMA_INC liblzma/lz)
list(APPEND LZMA_INC liblzma/lzma)
list(APPEND LZMA_INC liblzma/rangecoder)
list(APPEND LZMA_INC liblzma/simple)
list(APPEND LZMA_DEF -DLZMA_API_STATIC)

target_include_directories(lzma PRIVATE ${LZMA_INC})
target_compile_definitions(lzma PRIVATE ${LZMA_DEF})

list(APPEND ARCHIVE_INC liblzma/api)

add_library(lz4 STATIC 
  lz4/lz4.c
  lz4/lz4frame.c
  lz4/lz4hc.c
  lz4/xxhash.c
)
list(APPEND ARCHIVE_INC lz4)

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

file(GLOB ZSTD_COMMON_SOURCES zstd/common/*.c)
file(GLOB ZSTD_COMPRESS_SOURCES zstd/compress/*.c)
file(GLOB ZSTD_DECOMPRESS_SOURCES zstd/decompress/*.c)
file(GLOB ZSTD_DICTBUILDER_SOURCES zstd/dictBuilder/*.c)
file(GLOB ZSTD_DEPRECATED_SOURCES zstd/deprecated/*.c)


add_library(
  zstd STATIC
  ${ZSTD_COMMON_SOURCES}
  ${ZSTD_COMPRESS_SOURCES}
  ${ZSTD_DECOMPRESS_SOURCES}
  ${ZSTD_DICTBUILDER_SOURCES}
  ${ZSTD_DEPRECATED_SOURCES}
  zstd/legacy/zstd_v01.c
  zstd/legacy/zstd_v02.c
  zstd/legacy/zstd_v03.c
  zstd/legacy/zstd_v04.c
  zstd/legacy/zstd_v05.c
  zstd/legacy/zstd_v06.c
  zstd/legacy/zstd_v07.c)

target_compile_definitions(zstd PRIVATE XXH_NAMESPACE=_zstd_)
target_compile_definitions(zstd PRIVATE ZSTD_MULTITHREAD)
target_include_directories(zstd PRIVATE zstd zstd/common zstd/legacy)
list(APPEND ARCHIVE_INC zstd)

set(INCLUDE_HEADERS libarchive/archive.h libarchive/archive_entry.h)

# Sources and private headers
set(LIBARCHIVE_SOURCES
    libarchive/archive_acl.c
    libarchive/archive_acl_private.h
    libarchive/archive_check_magic.c
    libarchive/archive_cmdline.c
    libarchive/archive_cmdline_private.h
    libarchive/archive_crc32.h
    libarchive/archive_cryptor.c
    libarchive/archive_cryptor_private.h
    libarchive/archive_digest.c
    libarchive/archive_digest_private.h
    libarchive/archive_endian.h
    libarchive/archive_entry.c
    libarchive/archive_entry.h
    libarchive/archive_entry_copy_stat.c
    libarchive/archive_entry_link_resolver.c
    libarchive/archive_entry_locale.h
    libarchive/archive_entry_private.h
    libarchive/archive_entry_sparse.c
    libarchive/archive_entry_stat.c
    libarchive/archive_entry_strmode.c
    libarchive/archive_entry_xattr.c
    libarchive/archive_getdate.c
    libarchive/archive_getdate.h
    libarchive/archive_hmac.c
    libarchive/archive_hmac_private.h
    libarchive/archive_match.c
    libarchive/archive_openssl_evp_private.h
    libarchive/archive_openssl_hmac_private.h
    libarchive/archive_options.c
    libarchive/archive_options_private.h
    libarchive/archive_pack_dev.h
    libarchive/archive_pack_dev.c
    libarchive/archive_pathmatch.c
    libarchive/archive_pathmatch.h
    libarchive/archive_platform.h
    libarchive/archive_platform_acl.h
    libarchive/archive_platform_xattr.h
    libarchive/archive_ppmd_private.h
    libarchive/archive_ppmd8.c
    libarchive/archive_ppmd8_private.h
    libarchive/archive_ppmd7.c
    libarchive/archive_ppmd7_private.h
    libarchive/archive_private.h
    libarchive/archive_random.c
    libarchive/archive_random_private.h
    libarchive/archive_rb.c
    libarchive/archive_rb.h
    libarchive/archive_read.c
    libarchive/archive_read_add_passphrase.c
    libarchive/archive_read_append_filter.c
    libarchive/archive_read_data_into_fd.c
    libarchive/archive_read_disk_entry_from_file.c
    libarchive/archive_read_disk_posix.c
    libarchive/archive_read_disk_private.h
    libarchive/archive_read_disk_set_standard_lookup.c
    libarchive/archive_read_extract.c
    libarchive/archive_read_extract2.c
    libarchive/archive_read_open_fd.c
    libarchive/archive_read_open_file.c
    libarchive/archive_read_open_filename.c
    libarchive/archive_read_open_memory.c
    libarchive/archive_read_private.h
    libarchive/archive_read_set_format.c
    libarchive/archive_read_set_options.c
    libarchive/archive_read_support_filter_all.c
    libarchive/archive_read_support_filter_bzip2.c
    libarchive/archive_read_support_filter_compress.c
    libarchive/archive_read_support_filter_gzip.c
    libarchive/archive_read_support_filter_grzip.c
    libarchive/archive_read_support_filter_lrzip.c
    libarchive/archive_read_support_filter_lz4.c
    libarchive/archive_read_support_filter_lzop.c
    libarchive/archive_read_support_filter_none.c
    libarchive/archive_read_support_filter_program.c
    libarchive/archive_read_support_filter_rpm.c
    libarchive/archive_read_support_filter_uu.c
    libarchive/archive_read_support_filter_xz.c
    libarchive/archive_read_support_filter_zstd.c
    libarchive/archive_read_support_format_7zip.c
    libarchive/archive_read_support_format_all.c
    libarchive/archive_read_support_format_ar.c
    libarchive/archive_read_support_format_by_code.c
    libarchive/archive_read_support_format_cab.c
    libarchive/archive_read_support_format_cpio.c
    libarchive/archive_read_support_format_empty.c
    libarchive/archive_read_support_format_iso9660.c
    libarchive/archive_read_support_format_lha.c
    libarchive/archive_read_support_format_mtree.c
    libarchive/archive_read_support_format_rar.c
    libarchive/archive_read_support_format_rar5.c
    libarchive/archive_read_support_format_raw.c
    libarchive/archive_read_support_format_tar.c
    libarchive/archive_read_support_format_warc.c
    libarchive/archive_read_support_format_xar.c
    libarchive/archive_read_support_format_zip.c
    libarchive/archive_string.c
    libarchive/archive_string.h
    libarchive/archive_string_composition.h
    libarchive/archive_string_sprintf.c
    libarchive/archive_util.c
    libarchive/archive_version_details.c
    libarchive/archive_virtual.c
    libarchive/archive_write.c
    libarchive/archive_write_disk_posix.c
    libarchive/archive_write_disk_private.h
    libarchive/archive_write_disk_set_standard_lookup.c
    libarchive/archive_write_private.h
    libarchive/archive_write_open_fd.c
    libarchive/archive_write_open_file.c
    libarchive/archive_write_open_filename.c
    libarchive/archive_write_open_memory.c
    libarchive/archive_write_add_filter.c
    libarchive/archive_write_add_filter_b64encode.c
    libarchive/archive_write_add_filter_by_name.c
    libarchive/archive_write_add_filter_bzip2.c
    libarchive/archive_write_add_filter_compress.c
    libarchive/archive_write_add_filter_grzip.c
    libarchive/archive_write_add_filter_gzip.c
    libarchive/archive_write_add_filter_lrzip.c
    libarchive/archive_write_add_filter_lz4.c
    libarchive/archive_write_add_filter_lzop.c
    libarchive/archive_write_add_filter_none.c
    libarchive/archive_write_add_filter_program.c
    libarchive/archive_write_add_filter_uuencode.c
    libarchive/archive_write_add_filter_xz.c
    libarchive/archive_write_add_filter_zstd.c
    libarchive/archive_write_set_format.c
    libarchive/archive_write_set_format_7zip.c
    libarchive/archive_write_set_format_ar.c
    libarchive/archive_write_set_format_by_name.c
    libarchive/archive_write_set_format_cpio.c
    libarchive/archive_write_set_format_cpio_newc.c
    libarchive/archive_write_set_format_filter_by_ext.c
    libarchive/archive_write_set_format_gnutar.c
    libarchive/archive_write_set_format_iso9660.c
    libarchive/archive_write_set_format_mtree.c
    libarchive/archive_write_set_format_pax.c
    libarchive/archive_write_set_format_private.h
    libarchive/archive_write_set_format_raw.c
    libarchive/archive_write_set_format_shar.c
    libarchive/archive_write_set_format_ustar.c
    libarchive/archive_write_set_format_v7tar.c
    libarchive/archive_write_set_format_warc.c
    libarchive/archive_write_set_format_xar.c
    libarchive/archive_write_set_format_zip.c
    libarchive/archive_write_set_options.c
    libarchive/archive_write_set_passphrase.c
    libarchive/archive_xxhash.h
    libarchive/filter_fork_posix.c
    libarchive/filter_fork.h
    libarchive/xxhash.c)

list(APPEND LIBARCHIVE_SOURCES libarchive/archive_entry_copy_bhfi.c)
list(APPEND LIBARCHIVE_SOURCES libarchive/archive_read_disk_windows.c)
list(APPEND LIBARCHIVE_SOURCES libarchive/archive_windows.c)
list(APPEND LIBARCHIVE_SOURCES libarchive/archive_windows.h)
list(APPEND LIBARCHIVE_SOURCES libarchive/archive_write_disk_windows.c)
list(APPEND LIBARCHIVE_SOURCES libarchive/filter_fork_windows.c)
list(APPEND LIBARCHIVE_SOURCES libarchive/archive_blake2sp_ref.c)
list(APPEND LIBARCHIVE_SOURCES libarchive/archive_blake2s_ref.c)

# Libarchive is a shared library
add_library(archive STATIC ${LIBARCHIVE_SOURCES} ${INCLUDE_HEADERS})
target_include_directories(archive PRIVATE ${ARCHIVE_INC})
message(STATUS "ARCHIVE_INC ${ARCHIVE_INC}")
target_link_libraries(
  archive
  brotli
  bzip2
  lzma
  lz4
  zlib
  zstd)
