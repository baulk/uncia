////////////
#ifndef BAULKTAR_VERSION_H
#define BAULKTAR_VERSION_H

#ifdef APPVEYOR_BUILD_NUMBER
#define BAULKTAR_BUILD_NUMBER APPVEYOR_BUILD_NUMBER
#else
#define BAULKTAR_BUILD_NUMBER 1
#endif

#define TOSTR_1(x) L#x
#define TOSTR(x) TOSTR_1(x)

#define BAULKTARSUBVER TOSTR(BAULKTAR_BUILD_NUMBER)

#define BAULKTAR_MAJOR_VERSION 1
#define BAULKTAR_MINOR_VERSION 0
#define BAULKTAR_PATCH_VERSION 0

#define BAULKTAR_MAJOR TOSTR(BAULKTAR_MAJOR_VERSION)
#define BAULKTAR_MINOR TOSTR(BAULKTAR_MINOR_VERSION)
#define BAULKTAR_PATCH TOSTR(BAULKTAR_PATCH_VERSION)

#define BAULKTAR_VERSION_MAIN BAULKTAR_MAJOR L"." BAULKTAR_MINOR
#define BAULKTAR_VERSION_FULL BAULKTAR_VERSION_MAIN L"." BAULKTAR_PATCH

#ifdef APPVEYOR_BUILD_NUMBER
#define BAULKTAR_BUILD_VERSION                                                 \
  BAULKTAR_VERSION_FULL L"." BAULKTARSUBVER L" (appveyor)"
#else
#define BAULKTAR_BUILD_VERSION                                                 \
  BAULKTAR_VERSION_FULL L"." BAULKTARSUBVER L" (dev)"
#endif

#define BAULKTAR_APPLINK                                                       \
  L"For more information about this tool. \nVisit: <a "                        \
  L"href=\"https://github.com/baulk/baulktar\">BaulkTar</"                     \
  L"a>\nVisit: <a "                                                            \
  L"href=\"https://forcemz.net/\">forcemz.net</a>"

#define BAULKTAR_APPLINKE                                                      \
  L"Ask for help with this issue. \nVisit: <a "                                \
  L"href=\"https://github.com/baulk/baulktar/issues\">BaulkTar "               \
  L"Issues</a>"

#define BAULKTAR_APPVERSION                                                    \
  L"Version: " BAULKTAR_VERSION_FULL L"\nCopyright \xA9 2020, BaulkTar "       \
  L"contributors."

#define BAULKTAR_COPYRIGHT L"Copyright \xA9 2020, BaulkTar contributors."

#endif
