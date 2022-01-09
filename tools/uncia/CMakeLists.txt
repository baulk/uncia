#
file(GLOB UNCIA_SOURCES *.cc *.rc *.manifest)
add_executable(
  uncia
  ${UNCIA_SOURCES})

if(UNCIA_ENABLE_LTO)
  set_property(TARGET uncia PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_link_libraries(uncia archive belawin)
