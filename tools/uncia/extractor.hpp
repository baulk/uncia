///
#ifndef UNCIA_EXTRACTOR_HPP
#define UNCIA_EXTRACTOR_HPP
#include <bela/base.hpp>
#include <filesystem>

namespace uncia {
class Extractor {
public:
  Extractor(const std::filesystem::path &archive_file_, const std::filesystem::path &destination_,
            bool isDebugMode_ = false)
      : archive_file(archive_file_), destination(destination_), isDebugMode(isDebugMode_) {}
  Extractor(const Extractor &) = delete;
  Extractor &operator=(const Extractor &) = delete;
  bool Extract(bela::error_code &ec);

private:
  std::filesystem::path archive_file;
  std::filesystem::path destination;

  bool isDebugMode{false};
};
} // namespace uncia

#endif