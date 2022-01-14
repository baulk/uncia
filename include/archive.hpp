#include <bela/base.hpp>

namespace uncia::archive {
std::optional<std::string> JoinSanitizePath(std::wstring_view root, std::string_view filename,
                                             bool always_utf8 = true);
}