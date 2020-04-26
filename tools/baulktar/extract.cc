///
#include <bela/base.hpp>
#include <bela/path.hpp>
#include <bela/match.hpp>
#include <bela/strcat.hpp>
#include <bela/stdwriter.hpp>
#include <archive.h>
#include <archive_entry.h>
#include "baulktar.hpp"

namespace baulktar {
std::wstring BaulkWorkDir() {
  DWORD dwlen = MAX_PATH;
  std::wstring buf;
  for (;;) {
    buf.resize(dwlen);
    dwlen = GetCurrentDirectoryW(dwlen, buf.data());
    if (dwlen == 0) {
      return L"";
    }
    if (dwlen < buf.size()) {
      buf.resize(dwlen);
      break;
    }
  }
  return buf;
}
// In general, zip compression does not add a path prefix, and prescribed
// conventions such as tar do.
std::wstring MakeDestination(std::wstring_view file, std::wstring_view out) {
  if (!out.empty()) {
    return bela::PathAbsolute(out);
  }
  constexpr std::wstring_view allowsuffix[] = {L".zip", L".7z", L".rar"};
  for (const auto a : allowsuffix) {
    if (bela::EndsWithIgnoreCase(file, a)) {
      auto dest = BaulkWorkDir();
      auto name = bela::BaseName(file);
      name.remove_suffix(a.size());
      bela::StrAppend(&dest, L"\\", name); // $PWD/filename
      return dest;
    }
  }
  return BaulkWorkDir();
}

static int copy_data(struct archive *ar, struct archive *aw) {
  int r;
  const void *buff;
  size_t size;
  int64_t offset;

  for (;;) {
    if (r = archive_read_data_block(ar, &buff, &size, &offset);
        r == ARCHIVE_EOF) {
      return ARCHIVE_OK;
    }
    if (r != ARCHIVE_OK) {
      bela::FPrintF(stderr, L"archive_read_data_block: \x1b[31m%s\x1b[0m\n",
                    archive_error_string(ar));
      return r;
    }
    r = archive_write_data_block(aw, buff, size, offset);
    if (r != ARCHIVE_OK) {
      bela::FPrintF(stderr, L"archive_write_data_block: \x1b[31m%s\x1b[0m\n",
                    archive_error_string(ar));
      return r;
    }
  }
}

// https://github.com/libarchive/libarchive/wiki/Filenames
// Extract archive
int BaulkExtract(std::wstring_view file, std::wstring_view out) {
  auto src = bela::PathAbsolute(file);
  auto destination = MakeDestination(src, out);
  // The bsdtar extract file does not support setting the output directory, here
  // to improve
  if (!bela::PathExists(destination) &&
      CreateDirectoryW(destination.data(), nullptr) != TRUE) {
    auto ec = bela::make_system_error_code();
    bela::FPrintF(stderr, L"unable mkdir %s\n", ec.message);
    return 1;
  }
  if (SetCurrentDirectoryW(destination.data()) != TRUE) {
    auto ec = bela::make_system_error_code();
    bela::FPrintF(stderr, L"unable chdir %s\n", ec.message);
    return 1;
  }
  auto a = archive_read_disk_new();
  if (a == nullptr) {
    return 1;
  }
  int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM |
              ARCHIVE_EXTRACT_SECURE_NODOTDOT | ARCHIVE_EXTRACT_UNLINK |
              ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;
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
  if (archive_read_open_filename_w(a, src.data(), 16384) != ARCHIVE_OK) {
    return 1;
  }
  archive_entry *entry = nullptr;
  for (;;) {
    auto r = archive_read_next_header(a, &entry);
    if (r == ARCHIVE_EOF) {
      break;
    }
    if (r < ARCHIVE_OK) {
      bela::FPrintF(stderr, L"%s\n", archive_error_string(a));
      return 1;
    }
    if (r < ARCHIVE_WARN) {
      return 1;
    }
    if (baulktar::IsDebugMode) {
      bela::FPrintF(stderr, L"\x1b[33mx %s\x1b[0,\n",
                    archive_entry_pathname_w(entry));
    } else {
      bela::FPrintF(stderr, L"\x1b[2k\r\x1b[33mx %s\x1b[0m",
                    archive_entry_pathname_w(entry));
    }
    r = archive_write_header(ext, entry);
    if (r != ARCHIVE_OK) {
      break;
    }
    r = copy_data(a, ext);
    if (r != ARCHIVE_OK) {
      break;
    }
  }
  if (!baulktar::IsDebugMode) {
    bela::FPrintF(stderr, L"\n");
  }
  return 0;
}
} // namespace baulktar