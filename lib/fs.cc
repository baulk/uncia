//
#include <bela/path.hpp>
#include <bela/match.hpp>
#include <archive.hpp>
#include <bela/fs.hpp>
#include <filesystem>

namespace uncia::archive {
std::optional<std::wstring> find_unique_child_dir(std::wstring_view dir) {
  bela::fs::Finder finder;
  bela::error_code ec;
  if (!finder.First(dir, L"*", ec)) {
    return std::nullopt;
  }
  int count = 0;
  std::wstring subdir;
  do {
    if (finder.Ignore()) {
      continue;
    }
    if (!finder.IsDir()) {
      return std::nullopt;
    }
    if (subdir.empty()) {
      subdir = bela::StringCat(dir, L"\\", finder.Name());
    }
    count++;
  } while (finder.Next());

  if (count != 1) {
    return std::nullopt;
  }
  if (bela::EndsWithIgnoreCase(subdir, L"\\bin")) {
    return std::nullopt;
  }
  return std::make_optional(std::move(subdir));
}

bool MakeFlattened(std::wstring_view dir, std::wstring_view dest, bela::error_code &ec) {
  auto subfirst = find_unique_child_dir(dir);
  if (!subfirst) {
    return true;
  }
  std::wstring currentdir = *subfirst;
  for (int i = 0; i < 10; i++) {
    auto subdir_ = find_unique_child_dir(currentdir);
    if (!subdir_) {
      break;
    }
    currentdir.assign(std::move(*subdir_));
  }
  std::error_code e;
  std::filesystem::path destpath(dest);
  for (const auto &p : std::filesystem::directory_iterator(currentdir)) {
    auto newpath = destpath / p.path().filename();
    std::filesystem::rename(p.path(), newpath, e);
  }
  return bela::fs::ForceDeleteFolders(*subfirst, ec);
}

bool MakeDir(std::wstring_view path, bela::error_code &ec) {
  std::error_code e;
  if (std::filesystem::exists(path, e)) {
    return true;
  }
  if (!std::filesystem::create_directories(path, e)) {
    ec = e;
    return false;
  }
  return true;
}

constexpr bool is_dot_or_separator(wchar_t ch) { return bela::IsPathSeparator(ch) || ch == L'.'; }

std::wstring_view PathRemoveExtension(std::wstring_view p) {
  if (p.empty()) {
    return L".";
  }
  auto i = p.size() - 1;
  for (; i != 0 && is_dot_or_separator(p[i]); i--) {
    p.remove_suffix(1);
  }
  constexpr std::wstring_view extensions[] = {L".tar.gz",   L".tar.bz2", L".tar.xz", L".tar.zst",
                                              L".tar.zstd", L".tar.br",  L".tar.lz4"};
  for (const auto e : extensions) {
    if (bela::EndsWithIgnoreCase(p, e)) {
      return p.substr(0, p.size() - e.size());
    }
  }
  if (auto pos = p.find_last_of(L"\\/."); pos != std::wstring_view::npos && p[pos] == L'.') {
    // if (pos >= 1 && bela::IsPathSeparator(p[pos - 1])) {
    //   return p;
    // }
    return p.substr(0, pos);
  }
  return p;
}

} // namespace uncia::archive