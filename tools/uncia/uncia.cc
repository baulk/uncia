#include <string_view>
#include <bela/parseargv.hpp>
#include <archive.h>
#include <archive.hpp>
#include <version.hpp>
#include "extractor.hpp"
#include "uncia.hpp"

namespace uncia {
bool IsDebugMode = false;

class Executor {
public:
  Executor() = default;
  Executor(const Executor &) = delete;
  Executor &operator=(const Executor &) = delete;
  bool ParseArgv(int argc, wchar_t **argv, bela::error_code &ec);
  bool Execute(bela::error_code &ec);

private:
  bool make_flat(const std::filesystem::path &dest);
  std::vector<std::filesystem::path> archive_files;
  std::filesystem::path destination;
};

inline std::wstring_view strip_extension(const std::filesystem::path &filename) {
  return uncia::archive::PathStripExtension(filename.native());
}

std::optional<std::filesystem::path> make_unqiue_extracted_destination(const std::filesystem::path &archive_file,
                                                                       std::filesystem::path &strict_folder) {
  std::error_code e;
  auto parent_path = archive_file.parent_path();
  strict_folder = uncia::archive::PathStripExtension(archive_file.filename().native());
  auto d = parent_path / strict_folder;
  if (!std::filesystem::exists(d, e)) {
    return std::make_optional(std::move(d));
  }
  for (int i = 1; i < 100; i++) {
    d = parent_path / bela::StringCat(strict_folder, L"-(", i, L")");
    if (!std::filesystem::exists(d, e)) {
      return std::make_optional(std::move(d));
    }
  }
  return std::nullopt;
}

bool Executor::make_flat(const std::filesystem::path &dest) {
  bela::error_code ec;
  return uncia::archive::MakeFlattened(dest, ec);
}

bool Executor::Execute(bela::error_code &ec) {
  std::filesystem::path strict_folder;
  if (archive_files.size() == 1) {
    if (destination.empty()) {
      const auto &archive_file = archive_files[0];
      auto d = make_unqiue_extracted_destination(archive_file, strict_folder);
      if (!d) {
        ec = bela::make_error_code(bela::ErrGeneral, L"destination '", strict_folder, L"' already exists");
        return false;
      }
      destination = std::move(*d);
    }
    if (Extractor e(archive_files[0], destination, IsDebugMode); !e.Extract(ec)) {
      return false;
    }
    return make_flat(destination);
  }
  // Extracting multiple archives will ignore destination
  for (const auto &archive_file : archive_files) {
    auto destination_ = make_unqiue_extracted_destination(archive_file, strict_folder);
    if (!destination_) {
      continue;
    }
    if (Extractor e(archive_file, *destination_, IsDebugMode); !e.Extract(ec)) {
      return false;
    }
    make_flat(*destination_);
  }
  return true;
} // namespace baulk

void Usage() {
  constexpr wchar_t usage[] = LR"(Uncia - Simple archive extraction tool
Usage: unscrew [option] ...
  -h|--help        Show usage text and quit
  -v|--version     Show version number and quit
  -V|--verbose     Make the operation more talkative
  -d|--destination Set archive extracted destination (extracting multiple archives will be ignored)

)";
  bela::terminal::WriteAuto(stderr, usage);
}

void Version() {
  bela::FPrintF(stderr, L"Uncia - Simple archive extraction tool\nVersion:  %s\nDepends:  %s\n", UNCIA_VERSION,
                archive_version_details());
}

bool Executor::ParseArgv(int argc, wchar_t **argv, bela::error_code &ec) {
  std::error_code e;
  bela::ParseArgv pa(argc, argv);
  pa.Add(L"help", bela::no_argument, L'h')
      .Add(L"version", bela::no_argument, L'v')
      .Add(L"verbose", bela::no_argument, L'V')
      .Add(L"destination", bela::required_argument, L'd');
  auto ret = pa.Execute(
      [&](int val, const wchar_t *oa, const wchar_t *) {
        switch (val) {
        case 'h':
          Usage();
          exit(0);
        case 'v':
          Version();
          exit(0);
        case 'V':
          IsDebugMode = true;
          break;
        case 'd':
          destination = std::filesystem::absolute(oa, e);
          break;
        default:
          break;
        }
        return true;
      },
      ec);
  if (!ret) {
    return false;
  }
  for (const auto s : pa.UnresolvedArgs()) {
    archive_files.emplace_back(std::filesystem::absolute(s, e));
  }
  return true;
}

} // namespace uncia

int wmain(int argc, wchar_t **argv) {
  uncia::Executor e;
  bela::error_code ec;
  if (!e.ParseArgv(argc, argv, ec)) {
    bela::FPrintF(stderr, L"uncia parse argv error: \x1b[31m%s\x1b[0m\n", ec);
    return 1;
  }
  if (!e.Execute(ec)) {
    bela::FPrintF(stderr, L"uncia extract error: \x1b[31m%s\x1b[0m\n", ec);
    return 1;
  }
  return 0;
}