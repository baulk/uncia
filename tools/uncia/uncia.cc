#include <string_view>
#include <bela/parseargv.hpp>
#include <archive.h>
#include "uncia.hpp"
#include <version.hpp>

namespace uncia {
bool IsDebugMode = false;
void Version() { bela::FPrintF(stderr, L"uncia %s %s\n", UNCIA_VERSION, archive_version_details()); }

} // namespace uncia

int wmain(int argc, wchar_t **argv) {
  //
  uncia::Version();
  if (argc < 2) {
    bela::FPrintF(stderr, L"usage %s file\n", argv[0]);
    return 1;
  }
  auto dest = argc > 2 ? std::wstring(argv[2]) : uncia::FileDestination(argv[1]);
  return uncia::Extract(argv[1], dest);
}