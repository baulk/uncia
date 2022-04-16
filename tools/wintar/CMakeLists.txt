# wintar

file(
  GLOB
  WINTAR_SOURCES
  *.c
  *.rc
  *.manifest)

add_executable(wintar ${WINTAR_SOURCES})
if(UNCIA_ENABLE_LTO)
  set_property(TARGET wintar PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_link_libraries(wintar archive Bcrypt)

install(TARGETS wintar DESTINATION bin)
