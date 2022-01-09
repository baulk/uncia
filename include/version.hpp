///
#ifndef UNCIA_BAULKREV_HPP
#define UNCIA_BAULKREV_HPP

#ifdef __has_include
#if __has_include(<revision.h>)
#include <revision.h>
#endif
#endif

#ifndef UNCIA_VERSION_MAJOR
#define UNCIA_VERSION_MAJOR 0
#endif

#ifndef UNCIA_VERSION_MINOR
#define UNCIA_VERSION_MINOR 1
#endif

#ifndef UNCIA_VERSION_PATCH
#define UNCIA_VERSION_PATCH 0
#endif

#ifndef UNCIA_VERSION_BUILD
#define UNCIA_VERSION_BUILD 0
#endif

#ifndef UNCIA_VERSION
#define UNCIA_VERSION L"0.1.0"
#endif

#ifndef UNCIA_REVISION
#define UNCIA_REVISION L"none"
#endif

#ifndef UNCIA_REFNAME
#define UNCIA_REFNAME L"none"
#endif

#ifndef UNCIA_BUILD_TIME
#define UNCIA_BUILD_TIME L"none"
#endif

#ifndef UNCIA_REMOTE_URL
#define UNCIA_REMOTE_URL L"none"
#endif

#ifndef UNCIA_APPLINK
#define UNCIA_APPLINK                                                                                                  \
  L"For more information about this tool. \nVisit: <a "                                                                \
  L"href=\"https://github.com/baulk/uncia\">Baulk</"                                                                   \
  L"a>\nVisit: <a "                                                                                                    \
  L"href=\"https://forcemz.net/\">forcemz.net</a>"
#endif

#ifndef UNCIA_APPLINKE
#define UNCIA_APPLINKE                                                                                                 \
  L"Ask for help with this issue. \nVisit: <a "                                                                        \
  L"href=\"https://github.com/baulk/uncia/issues\">Baulk "                                                             \
  L"Issues</a>"
#endif

#ifndef UNCIA_APPVERSION
#define UNCIA_APPVERSION L"Version: 0.1.0\nCopyright \xA9 2022, Baulk contributors."
#endif

#ifndef UNCIA_COPYRIGHT
#define UNCIA_COPYRIGHT L"Copyright \xA9 2022, Baulk contributors."
#endif

#endif