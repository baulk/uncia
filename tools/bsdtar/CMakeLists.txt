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
    passphrase.c
    bsdtar.rc)

if(WIN32 AND NOT CYGWIN)
  list(APPEND BSDTAR_SOURCES bsdtar_windows.c)
endif()

add_executable(bsdtar ${BSDTAR_SOURCES} bsdtar.manifest)
if(BAULKTAR_ENABLE_LTO)
  set_property(TARGET bsdtar PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_include_directories(bsdtar PRIVATE ../../lib/libarchive)
target_compile_definitions(bsdtar PRIVATE LIBARCHIVE_STATIC=1)
target_link_libraries(bsdtar archive Bcrypt)


add_executable(wintar ${BSDTAR_SOURCES} bsdtar.utf8.manifest)
if(BAULKTAR_ENABLE_LTO)
  set_property(TARGET wintar PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_include_directories(wintar PRIVATE ../../lib/libarchive)
target_compile_definitions(wintar PRIVATE LIBARCHIVE_STATIC=1 FORCE_ENABLE_UTF8=1)
target_link_libraries(wintar archive Bcrypt)

install(TARGETS bsdtar DESTINATION bin)
install(TARGETS wintar DESTINATION bin)
