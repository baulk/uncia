///
#include <bela/base.hpp>
#include <bela/path.hpp>
#include <bela/match.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <archive.hpp>
#include "uncia.hpp"

namespace uncia {
constexpr size_t block_size = 64 * 1024;

static int copy_data(struct archive *ar, struct archive *aw) {
  int r;
  const void *buff;
  size_t size;
  int64_t offset;

  for (;;) {
    if (r = archive_read_data_block(ar, &buff, &size, &offset); r == ARCHIVE_EOF) {
      return ARCHIVE_OK;
    }
    if (r != ARCHIVE_OK) {
      bela::FPrintF(stderr, L"archive_read_data_block: \x1b[31m%s\x1b[0m\n", archive_error_string(ar));
      return r;
    }
    auto wbytes = archive_write_data_block(aw, buff, size, offset);
    if (wbytes != ARCHIVE_OK) {
      bela::FPrintF(stderr, L"archive_write_data_block: \x1b[31m%s\x1b[0m\n", archive_error_string(ar));
      return static_cast<int>(wbytes);
    }
  }
}

// https://github.com/libarchive/libarchive/wiki/Filenames
// Extract archive
int extract_internal(std::wstring_view file, std::wstring_view out) {
  // The bsdtar extract file does not support setting the output directory, here
  // to improve
  bela::error_code ec;
  if (!uncia::archive::MakeDir(out, ec)) {
    bela::FPrintF(stderr, L"create %s error: \x1b[31m%s\x1b[0m\n", out, ec);
    return 1;
  }
  auto a = archive_read_new();
  if (a == nullptr) {
    bela::FPrintF(stderr, L"archive_read_new: error\n");
    return 1;
  }
  int flags = ARCHIVE_EXTRACT_SECURE_NODOTDOT | ARCHIVE_EXTRACT_SECURE_SYMLINKS | ARCHIVE_EXTRACT_UNLINK |
              ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;
  auto ext = archive_write_disk_new();
  archive_write_disk_set_options(ext, flags);
  archive_write_disk_set_standard_lookup(ext);
  auto closer = bela::finally([&] {
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
  });
  archive_read_support_compression_all(a);
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);
  archive_read_support_format_empty(a);
  if (archive_read_open_filename_w(a, file.data(), block_size) != ARCHIVE_OK) {
    bela::FPrintF(stderr, L"archive_read_open_filename_w: \x1b[31m%s\x1b[0m\n", archive_error_string(a));
    return 1;
  }
  auto pathNameFn = [&](std::string_view p) -> std::string_view {
    if (p.size() > out.size() + 1) {
      return p.substr(out.size() + 1);
    }
    return "*";
  };
  archive_entry *entry = nullptr;
  for (;;) {
    auto r = archive_read_next_header(a, &entry);
    if (r == ARCHIVE_EOF) {
      break;
    }
    if (r < ARCHIVE_OK) {
      bela::FPrintF(stderr, L"archive_read_next_header: %s\n", archive_error_string(a));
      return 1;
    }
    if (r < ARCHIVE_WARN) {
      return 1;
    }
    auto filePath = uncia::archive::JoinSanitizePath(out, archive_entry_pathname_utf8(entry));
    if (!filePath) {
      continue;
    }
    archive_entry_set_pathname_utf8(entry, filePath->data());
    if (IsDebugMode) {
      bela::FPrintF(stderr, L"\x1b[33mx %s\x1b[0m\n", pathNameFn(*filePath));
    } else {
      bela::FPrintF(stderr, L"\x1b[2K\r\x1b[33mx %s\x1b[0m", pathNameFn(*filePath));
    }
    r = archive_write_header(ext, entry);
    if (r != ARCHIVE_OK) {
      bela::FPrintF(stderr, L"archive_write_header: %s\n", archive_error_string(a));
      return 1;
    }
    r = copy_data(a, ext);
    if (r != ARCHIVE_OK) {
      return 1;
    }
  }
  if (!IsDebugMode) {
    bela::FPrintF(stderr, L"\n");
  }
  return 0;
}
int Extract(std::wstring_view file, std::wstring_view out) {
  auto absFile = bela::PathAbsolute(file);
  auto absOut = bela::PathAbsolute(out);
  if (auto exitcode = extract_internal(absFile, absOut); exitcode != 0) {
    return exitcode;
  }
  bela::error_code ec;
  if (!uncia::archive::MakeFlattened(absOut, absOut, ec)) {
    bela::FPrintF(stderr, L"unable MakeFlattened %s\n", ec);
  }
  return 0;
}

} // namespace uncia