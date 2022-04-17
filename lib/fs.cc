//
#include <bela/path.hpp>
#include <bela/match.hpp>
#include <bela/fs.hpp>
#include <bela/str_split_narrow.hpp>
#include <archive.hpp>
#include <filesystem>

namespace uncia::archive {

inline std::optional<std::filesystem::path> flattened_recursive(std::filesystem::path &current, std::error_code &e) {
  int entries = 0;
  std::filesystem::path folder0;
  for (const auto &entry : std::filesystem::directory_iterator{current, e}) {
    if (!entry.is_directory(e)) {
      return std::make_optional(current);
    }
    if (bela::EqualsIgnoreCase(entry.path().filename().native(), L"bin")) {
      return std::make_optional(current);
    }
    entries++;
    if (entries) {
      folder0 = entry.path();
    }
  }
  if (entries != 1) {
    return std::make_optional(current);
  }
  current = folder0;
  return std::nullopt;
}

std::optional<std::filesystem::path> flattened_internal(const std::filesystem::path &d, std::filesystem::path &depth1) {
  std::error_code e;
  if (!std::filesystem::is_directory(d, e)) {
    return std::nullopt;
  }
  std::filesystem::path current{std::filesystem::absolute(d, e)};
  for (int i = 0; i < 20; i++) {
    if (auto flatd = flattened_recursive(current, e); flatd) {
      return flatd;
    }
    if (depth1.empty()) {
      depth1 = current;
    }
  }
  return std::nullopt;
}

std::optional<std::filesystem::path> Flattened(const std::filesystem::path &d) {
  std::filesystem::path unused;
  return flattened_internal(d, unused);
}

bool MakeFlattened(const std::filesystem::path &d, bela::error_code &ec) {
  std::error_code e;
  std::filesystem::path depth1;
  auto flat = flattened_internal(d, depth1);
  if (!flat) {
    ec = bela::make_error_code(L"no conditions for a flattened path");
    return false;
  }
  if (std::filesystem::equivalent(d, *flat, e)) {
    return true;
  }
  for (const auto &entry : std::filesystem::directory_iterator{*flat, e}) {
    auto newPath = d / entry.path().filename();
    std::filesystem::rename(entry.path(), newPath, e);
  }
  if (!depth1.empty()) {
    std::filesystem::remove_all(depth1, e);
  }
  return true;
}

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

std::wstring_view PathStripExtension(std::wstring_view p) {
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
    return p.substr(0, pos);
  }
  return p;
}

std::optional<std::filesystem::path> MakeUnqiueExtractedDestination(const std::filesystem::path &archive_file,
                                                                       std::filesystem::path &strict_folder) {
  std::error_code e;
  auto parent_path = archive_file.parent_path();
  strict_folder = PathStripExtension(archive_file.filename().native());
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

} // namespace uncia::archive