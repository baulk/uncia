#include <string_view>
#include <bela/parseargv.hpp>
#include <archive.h>
#include "uncia.hpp"
#include <version.hpp>

namespace uncia {
bool IsDebugMode = false;

void Version() { bela::FPrintF(stderr, L"uncia %s %s\n", BAULKTAR_VERSION, archive_version_details()); }
// -x --extract
// -o --output
// -h --help
// -v --version
// -V --verbose
// -c --create
// -f --format
// -z, -j, -J, --lzma --zstd
// -l --list
// --exclude
} // namespace uncia

int wmain(int argc, wchar_t **argv) {
  //
  uncia::Version();
  return 0;
}