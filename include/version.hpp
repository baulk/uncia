///
#ifndef BAULKTAR_BAULKREV_HPP
#define BAULKTAR_BAULKREV_HPP

#ifdef __has_include
#if __has_include(<revision.h>)
#include <revision.h>
#endif
#endif

#ifndef BAULKTAR_VERSION_MAJOR
#define BAULKTAR_VERSION_MAJOR 0
#endif

#ifndef BAULKTAR_VERSION_MINOR
#define BAULKTAR_VERSION_MINOR 1
#endif

#ifndef BAULKTAR_VERSION_PATCH
#define BAULKTAR_VERSION_PATCH 0
#endif

#ifndef BAULKTAR_VERSION_BUILD
#define BAULKTAR_VERSION_BUILD 0
#endif

#ifndef BAULKTAR_VERSION
#define BAULKTAR_VERSION L"0.1.0"
#endif

#ifndef BAULKTAR_REVISION
#define BAULKTAR_REVISION L"none"
#endif

#ifndef BAULKTAR_REFNAME
#define BAULKTAR_REFNAME L"none"
#endif

#ifndef BAULKTAR_BUILD_TIME
#define BAULKTAR_BUILD_TIME L"none"
#endif

#ifndef BAULKTAR_REMOTE_URL
#define BAULKTAR_REMOTE_URL L"none"
#endif

#ifndef BAULKTAR_APPLINK
#define BAULKTAR_APPLINK                                                                                                  \
  L"For more information about this tool. \nVisit: <a "                                                                \
  L"href=\"https://github.com/baulk/baulktar\">Baulk</"                                                                   \
  L"a>\nVisit: <a "                                                                                                    \
  L"href=\"https://forcemz.net/\">forcemz.net</a>"
#endif

#ifndef BAULKTAR_APPLINKE
#define BAULKTAR_APPLINKE                                                                                                 \
  L"Ask for help with this issue. \nVisit: <a "                                                                        \
  L"href=\"https://github.com/baulk/baulktar/issues\">Baulk "                                                             \
  L"Issues</a>"
#endif

#ifndef BAULKTAR_APPVERSION
#define BAULKTAR_APPVERSION L"Version: 0.1.0\nCopyright \xA9 2022, Baulk contributors."
#endif

#ifndef BAULKTAR_COPYRIGHT
#define BAULKTAR_COPYRIGHT L"Copyright \xA9 2022, Baulk contributors."
#endif

#endif