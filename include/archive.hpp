#include <bela/base.hpp>
#include <bela/str_cat.hpp>
#include <filesystem>

namespace uncia::archive {
// Flattened: find flatten child directories
std::optional<std::filesystem::path> Flattened(const std::filesystem::path &d);
// MakeFlattened: Flatten directories
bool MakeFlattened(const std::filesystem::path &d, bela::error_code &ec);

inline bool MakeDirectories(const std::filesystem::path &path, bela::error_code &ec) {
  std::error_code e;
  if (std::filesystem::exists(path, e)) {
    return true;
  }
  if (std::filesystem::create_directories(path, e); e) {
    ec = bela::make_error_code_from_std(e);
    return false;
  }
  return true;
}

inline bool MakeParentDirectories(const std::filesystem::path &path, bela::error_code &ec) {
  std::error_code e;
  auto parent = path.parent_path();
  if (std::filesystem::exists(parent, e)) {
    return true;
  }
  if (std::filesystem::create_directories(parent, e); e) {
    ec = bela::make_error_code_from_std(e);
    return false;
  }
  return true;
}
std::wstring_view PathStripExtension(std::wstring_view p);
std::optional<std::filesystem::path> MakeUnqiueExtractedDestination(const std::filesystem::path &archive_file,
                                                                    std::filesystem::path &strict_folder);
std::optional<std::filesystem::path> JoinSanitizeFsPath(const std::filesystem::path &root, std::string_view child_path,
                                                        bool always_utf8, std::wstring &encoded_path);
} // namespace uncia::archive