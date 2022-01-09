#
file(GLOB UNCIA_SOURCES *.cc *.rc *.manifest)
add_executable(
  uncia
  ${UNCIA_SOURCES})

if(BAULKTAR_ENABLE_LTO)
  set_property(TARGET uncia PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_include_directories(uncia PRIVATE ../../lib/libarchive ../../vendor/bela/include)
target_compile_definitions(uncia PRIVATE LIBARCHIVE_STATIC=1)
target_link_libraries(uncia archive belawin)
