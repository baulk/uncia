#

add_executable(btar btar.cc filter.cc btar.rc)

target_link_libraries(btar archive)
