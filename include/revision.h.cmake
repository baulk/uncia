// Generate code by cmake, don't modify
#ifndef BAULKTAR_VERSION_H
#define BAULKTAR_VERSION_H

#define BAULKTAR_VERSION_MAJOR ${BAULKTAR_VERSION_MAJOR}
#define BAULKTAR_VERSION_MINOR ${BAULKTAR_VERSION_MINOR}
#define BAULKTAR_VERSION_PATCH ${BAULKTAR_VERSION_PATCH}
#define BAULKTAR_VERSION_BUILD ${BAULKTAR_VERSION_BUILD}

#define BAULKTAR_VERSION L"${BAULKTAR_VERSION_MAJOR}.${BAULKTAR_VERSION_MINOR}.${BAULKTAR_VERSION_PATCH}"
#define BAULKTAR_REVISION L"${BAULKTAR_REVISION}"
#define BAULKTAR_REFNAME L"${BAULKTAR_REFNAME}"
#define BAULKTAR_BUILD_TIME L"${BAULKTAR_BUILD_TIME}"
#define BAULKTAR_REMOTE_URL L"${BAULKTAR_REMOTE_URL}"

#define BAULKTAR_APPLINK                                                                              \
  L"For more information about this tool. \nVisit: <a "                                            \
  L"href=\"https://github.com/baulk/baulktar\">Baulk</"                                               \
  L"a>\nVisit: <a "                                                                                \
  L"href=\"https://forcemz.net/\">forcemz.net</a>"

#define BAULKTAR_APPLINKE                                                                             \
  L"Ask for help with this issue. \nVisit: <a "                                                    \
  L"href=\"https://github.com/baulk/baulktar/issues\">Baulk "                                         \
  L"Issues</a>"

#define BAULKTAR_APPVERSION                                                                           \
  L"Version: ${BAULKTAR_VERSION_MAJOR}.${BAULKTAR_VERSION_MINOR}.${BAULKTAR_VERSION_PATCH}\nCopyright "     \
  L"\xA9 ${BAULKTAR_COPYRIGHT_YEAR}, Baulk contributors."

#define BAULKTAR_COPYRIGHT L"Copyright \xA9 ${BAULKTAR_COPYRIGHT_YEAR}, Baulk contributors."

#ifndef FORCE_ENABLE_UTF8
#define BSDATR_FILE_DESCRIPTION L"bsdtar archive tool"
#else
#define BSDATR_FILE_DESCRIPTION L"wintar - UTF-8 mode bsdtar archive tool"
#endif

#endif