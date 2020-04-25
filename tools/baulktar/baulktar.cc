#include <string_view>
#include <bela/parseargv.hpp>
#include <archive.h>
#include "baulktar.hpp"
#include "version.h"

namespace baulktar {
bool IsDebugMode = false;

void Version() {
  bela::FPrintF(stderr, L"baulktar %s %s\n", BAULKTAR_VERSION_FULL,
                archive_version_details());
}
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
} // namespace baulktar

int wmain(int argc, wchar_t **argv) {
  //
  return 0;
}