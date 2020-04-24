# bsdtar
set(BSDTAR_SOURCES
  bsdtar.c
  cmdline.c
  creation_set.c
  read.c
  subst.c
  util.c
  write.c
  err.c
  line_reader.c
  passphrase.c)

if(WIN32 AND NOT CYGWIN)
list(APPEND BSDTAR_SOURCES bsdtar_windows.c)
endif()

add_executable(bsdtar ${BSDTAR_SOURCES})
target_include_directories(bsdtar PRIVATE ../../lib/libarchive)
target_compile_definitions(bsdtar PRIVATE LIBARCHIVE_STATIC=1)
target_link_libraries(bsdtar archive Bcrypt)

