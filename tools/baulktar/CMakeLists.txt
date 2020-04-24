#

add_executable(baulktar baulktar.cc filter.cc baulktar.rc baulktar.manifest)

target_link_libraries(baulktar archive)
