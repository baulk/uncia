#include <bela/base.hpp>
#include <bela/str_cat.hpp>

namespace uncia::archive {
std::optional<std::string> JoinSanitizePath(std::wstring_view root, std::string_view filename, bool always_utf8 = true);
bool MakeFlattened(std::wstring_view dir, std::wstring_view dest, bela::error_code &ec);
bool MakeDir(std::wstring_view path, bela::error_code &ec);
std::wstring_view PathRemoveExtension(std::wstring_view p);
inline std::wstring FileDestination(std::wstring_view arfile) {
  if (auto d = PathRemoveExtension(arfile); d.size() != arfile.size()) {
    return std::wstring(d);
  }
  return bela::StringCat(arfile, L".out");
}
} // namespace uncia::archive