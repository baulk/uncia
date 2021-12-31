////////////
#ifndef BSDTAR_VERSION_H
#define BSDTAR_VERSION_H

#ifdef APPVEYOR_BUILD_NUMBER
#define BSDTAR_BUILD_NUMBER APPVEYOR_BUILD_NUMBER
#else
#define BSDTAR_BUILD_NUMBER 1
#endif

#define TOSTR_1(x) L#x
#define TOSTR(x) TOSTR_1(x)

#define BSDTARSUBVER TOSTR(BSDTAR_BUILD_NUMBER)

#define BSDTAR_MAJOR_VERSION 3
#define BSDTAR_MINOR_VERSION 4
#define BSDTAR_PATCH_VERSION 4

#define BSDTAR_MAJOR TOSTR(BSDTAR_MAJOR_VERSION)
#define BSDTAR_MINOR TOSTR(BSDTAR_MINOR_VERSION)
#define BSDTAR_PATCH TOSTR(BSDTAR_PATCH_VERSION)

#define BSDTAR_VERSION_MAIN BSDTAR_MAJOR L"." BSDTAR_MINOR
#define BSDTAR_VERSION_FULL BSDTAR_VERSION_MAIN L"." BSDTAR_PATCH

#ifdef APPVEYOR_BUILD_NUMBER
#define BSDTAR_BUILD_VERSION                                                 \
  BSDTAR_VERSION_FULL L"." BSDTARSUBVER L" (appveyor)"
#else
#define BSDTAR_BUILD_VERSION                                                 \
  BSDTAR_VERSION_FULL L"." BSDTARSUBVER L" (dev)"
#endif

#ifndef FORCE_ENABLE_UTF8
#define BSDATR_FILE_DESCRIPTION L"bsdtar archive tool"
#else
#define BSDATR_FILE_DESCRIPTION L"wintar - UTF-8 mode bsdtar archive tool"
#endif

#define BSDTAR_COPYRIGHT L"Copyright \xA9 2022, libarchive and baulk contributors."

#endif
