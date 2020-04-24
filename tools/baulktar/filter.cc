#include <cstdio>
#include <cstring>
#include <string_view>

struct suffix_code_t {
  const char *suffix;
  const char *form;
};

static const char *get_filter_code(const char *suffix) {
  /* A pair of suffix and compression/filter. */
  static constexpr const struct suffix_code_t filters[] = {
      {".Z", "compress"},  {".bz2", "bzip2"}, {".gz", "gzip"},
      {".grz", "grzip"},   {".lrz", "lrzip"}, {".lz", "lzip"},
      {".lz4", "lz4"},     {".lzo", "lzop"},  {".lzma", "lzma"},
      {".uu", "uuencode"}, {".xz", "xz"},     {".zst", "zstd"}};

  for (const auto &filter : filters) {
    if (std::strcmp(filter.suffix, suffix)) {
      return filter.form;
    }
  }
  return nullptr;
}

static const char *get_format_code(const char *suffix) {
  /* A pair of suffix and format. */
  static constexpr const struct suffix_code_t formats[] = {
      {".7z", "7zip"},     {".ar", "arbsd"},    {".cpio", "cpio"},
      {".iso", "iso9960"}, {".mtree", "mtree"}, {".shar", "shar"},
      {".tar", "paxr"},    {".warc", "warc"},   {".xar", "xar"},
      {".zip", "zip"}};
  for (const auto &fmt : formats) {
    if (std::strcmp(fmt.suffix, suffix)) {
      return fmt.form;
    }
  }
  return nullptr;
}

static const char *decompose_alias(const char *suffix) {
  static constexpr const struct suffix_code_t alias[] = {
      {".taz", ".tar.gz"},   {".tgz", ".tar.gz"},  {".tbz", ".tar.bz2"},
      {".tbz2", ".tar.bz2"}, {".tz2", ".tar.bz2"}, {".tlz", ".tar.lzma"},
      {".txz", ".tar.xz"},   {".tzo", ".tar.lzo"}, {".taZ", ".tar.Z"},
      {".tZ", ".tar.Z"},     {".tzst", ".tar.zst"}};
  for (const auto &a : alias) {
    if (std::strcmp(a.suffix, suffix)) {
      return a.form;
    }
  }
  return nullptr;
}