# bsdtar

file(GLOB BSDTAR_SOURCES *.c)

add_executable(bsdtar ${BSDTAR_SOURCES} bsdtar.manifest)
if(BAULKTAR_ENABLE_LTO)
  set_property(TARGET bsdtar PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_link_libraries(bsdtar archive Bcrypt)


add_executable(wintar ${BSDTAR_SOURCES} bsdtar.utf8.manifest)
if(BAULKTAR_ENABLE_LTO)
  set_property(TARGET wintar PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_compile_definitions(wintar PRIVATE FORCE_ENABLE_UTF8=1)
target_link_libraries(wintar archive Bcrypt)

install(TARGETS bsdtar DESTINATION bin)
install(TARGETS wintar DESTINATION bin)
