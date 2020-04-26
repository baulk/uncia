///
#include <bela/base.hpp>
#include <bela/path.hpp>
#include <bela/match.hpp>
#include <bela/strcat.hpp>
#include <bela/stdwriter.hpp>
#include <archive.h>
#include <archive_entry.h>
// We will change
//#include <archive_private.h>
//#include <archive_entry_private.h>

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

static int copy_data(struct archive *ar, HANDLE FileHandle) {
  int r;
  const void *buff;
  size_t size;
  la_int64_t offset;
  DWORD dwBytes = 0;
  for (;;) {
    if (r = archive_read_data_block(ar, &buff, &size, &offset);
        r == ARCHIVE_EOF) {
      return ARCHIVE_OK;
    }
    if (r < ARCHIVE_OK) {
      return r;
    }
    //archive_write_data_block()
    if (WriteFile(FileHandle, buff, static_cast<DWORD>(size), &dwBytes,
                  nullptr) != TRUE) {
      return ARCHIVE_FATAL;
    }
  }
  return ARCHIVE_OK;
}
// https://github.com/libarchive/libarchive/wiki/Filenames

int BaulkExtract(std::wstring_view file, std::wstring_view out) {
  auto src = bela::PathAbsolute(file);
  auto destination = MakeDestination(src, out);
  auto a = archive_read_disk_new();
  if (a == nullptr) {
    return 1;
  }
  auto ext = archive_write_disk_new();
  auto closer = bela::finally([&] { archive_read_free(a); });
  archive_read_support_compression_all(a);
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);
  archive_read_support_format_empty(a);
  if (archive_read_open_filename_w(a, src.data(), 16384) != ARCHIVE_OK) {
    return 1;
  }
  // archive_write_disk_set_options()
  archive_entry *entry = nullptr;
  for (;;) {
    auto r = archive_read_next_header(a, &entry);
    if (r == ARCHIVE_EOF) {
      break;
    }
    if (r < ARCHIVE_OK) {
      bela::FPrintF(stderr, L"%s\n", archive_error_string(a));
    }
    if (r < ARCHIVE_WARN) {
      return 1;
    }
    auto p = archive_entry_pathname(entry);
    //archive_entry_copy_pathname_w()
    // archive_entry_pathname_w
    // archive_entry_pathname_utf8
  }
  return 0;
}
} // namespace baulktar