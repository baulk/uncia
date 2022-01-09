//
#ifndef UNCIA_HPP
#define UNCIA_HPP
#include <bela/terminal.hpp>

namespace uncia {
extern bool IsDebugMode;
extern bool IsForceMode;
// DbgPrint added newline
// DbgPrint added newline
template <typename... Args> bela::ssize_t DbgPrint(const wchar_t *fmt, const Args &...args) {
  if (!IsDebugMode) {
    return 0;
  }
  const bela::format_internal::FormatArg arg_array[] = {args...};
  std::wstring str;
  str.append(L"\x1b[33m* ");
  bela::format_internal::StrAppendFormatInternal(&str, fmt, arg_array, sizeof...(args));
  if (str.back() == '\n') {
    str.pop_back();
  }
  str.append(L"\x1b[0m\n");
  return bela::terminal::WriteAuto(stderr, str);
}
inline bela::ssize_t DbgPrint(const wchar_t *fmt) {
  if (!IsDebugMode) {
    return 0;
  }
  std::wstring_view msg(fmt);
  if (!msg.empty() && msg.back() == '\n') {
    msg.remove_suffix(1);
  }
  return bela::terminal::WriteAuto(stderr, bela::StringCat(L"\x1b[33m* ", msg, L"\x1b[0m\n"));
}
std::wstring_view PathRemoveExtension(std::wstring_view p);
inline std::wstring FileDestination(std::wstring_view arfile) {
  if (auto d = PathRemoveExtension(arfile); d.size() != arfile.size()) {
    return std::wstring(d);
  }
  return bela::StringCat(arfile, L".out");
}
bool MakeDir(std::wstring_view path, bela::error_code &ec);
bool MakeFlattened(std::wstring_view dir, std::wstring_view dest, bela::error_code &ec);
int Extract(std::wstring_view file, std::wstring_view out);
} // namespace uncia

#endif