#include <archive.h>
#include <cstdio>

void dump() {
  fprintf(stderr, "lz4 %s\n", archive_liblz4_version());
  fprintf(stderr, "liblzma %s\n", archive_liblzma_version());
  fprintf(stderr, "zlib %s\n", archive_zlib_version());
  fprintf(stderr, "libzstd %s\n", archive_libzstd_version());
  fprintf(stderr, "details: %s\n", archive_version_details());
}