// Generate code by cmake, don't modify
#ifndef UNCIA_VERSION_H
#define UNCIA_VERSION_H

#define UNCIA_VERSION_MAJOR ${UNCIA_VERSION_MAJOR}
#define UNCIA_VERSION_MINOR ${UNCIA_VERSION_MINOR}
#define UNCIA_VERSION_PATCH ${UNCIA_VERSION_PATCH}
#define UNCIA_VERSION_BUILD ${UNCIA_VERSION_BUILD}

#define UNCIA_VERSION L"${UNCIA_VERSION_MAJOR}.${UNCIA_VERSION_MINOR}.${UNCIA_VERSION_PATCH}"
#define UNCIA_REVISION L"${UNCIA_REVISION}"
#define UNCIA_REFNAME L"${UNCIA_REFNAME}"
#define UNCIA_BUILD_TIME L"${UNCIA_BUILD_TIME}"
#define UNCIA_REMOTE_URL L"${UNCIA_REMOTE_URL}"

#define UNCIA_APPLINK                                                                              \
  L"For more information about this tool. \nVisit: <a "                                            \
  L"href=\"https://github.com/baulk/uncia\">Baulk</"                                               \
  L"a>\nVisit: <a "                                                                                \
  L"href=\"https://forcemz.net/\">forcemz.net</a>"

#define UNCIA_APPLINKE                                                                             \
  L"Ask for help with this issue. \nVisit: <a "                                                    \
  L"href=\"https://github.com/baulk/uncia/issues\">Baulk "                                         \
  L"Issues</a>"

#define UNCIA_APPVERSION                                                                           \
  L"Version: ${UNCIA_VERSION_MAJOR}.${UNCIA_VERSION_MINOR}.${UNCIA_VERSION_PATCH}\nCopyright "     \
  L"\xA9 ${UNCIA_COPYRIGHT_YEAR}, Baulk contributors."

#define UNCIA_COPYRIGHT L"Copyright \xA9 ${UNCIA_COPYRIGHT_YEAR}, Baulk contributors."

#endif