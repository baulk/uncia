///
#include <archive.h>
#include <archive_entry.h>
#include <bela/terminal.hpp>
#include <archive.hpp>
#include "extractor.hpp"

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

constexpr int archive_flags = ARCHIVE_EXTRACT_SECURE_NODOTDOT | ARCHIVE_EXTRACT_SECURE_SYMLINKS |
                              ARCHIVE_EXTRACT_UNLINK | ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM |
                              ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;

bool Extractor::Extract(bela::error_code &ec) {
  auto a = archive_read_new();
  if (a == nullptr) {
    ec = bela::make_error_code(L"allocate archive context failed");
    return false;
  }
  auto ext = archive_write_disk_new();
  archive_write_disk_set_options(ext, archive_flags);
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
  if (archive_read_open_filename_w(a, archive_file.c_str(), block_size) != ARCHIVE_OK) {
    ec = bela::make_error_code(bela::ErrGeneral, L"open archive ",
                               bela::encode_into<char, wchar_t>(archive_error_string(a)));
    return 1;
  }
  std::wstring encoded_path;
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
    auto filePath = archive::JoinSanitizeFsPath(destination, archive_entry_pathname_utf8(entry), true, encoded_path);
    if (!filePath) {
      continue;
    }
    auto narrowPath = bela::encode_into<wchar_t, char>(filePath->native());
    archive_entry_set_pathname_utf8(entry, narrowPath.data());
    if (isDebugMode) {
      bela::FPrintF(stderr, L"\x1b[33mx %s\x1b[0m\n", encoded_path);
    } else {
      bela::FPrintF(stderr, L"\x1b[2K\r\x1b[33mx %s\x1b[0m", encoded_path);
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
  if (!isDebugMode) {
    bela::FPrintF(stderr, L"\n");
  }
  return true;
}
} // namespace uncia