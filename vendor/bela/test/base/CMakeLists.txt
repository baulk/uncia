# base
add_executable(base_test
  base.cc
)

target_link_libraries(base_test
  bela
)

# base
add_executable(cityhash_test
  cityhash.cc
)

target_link_libraries(cityhash_test
  bela
)


# base
add_executable(ascii_test
  ascii.cc
)

target_link_libraries(ascii_test
  bela
)

# base
add_executable(fnmatch_test
  fnmatch.cc
)

target_link_libraries(fnmatch_test
  bela
)

# delfile
add_executable(delfile_test
  delfile.cc
)

target_link_libraries(delfile_test
  bela
  belawin
)

# base
add_executable(strsplit_test
  strsplit.cc
)

target_link_libraries(strsplit_test
  bela
)



# base
add_executable(startswith_test
  startswith.cc
)

target_link_libraries(startswith_test
  bela
)

# base
add_executable(static_string_test
  static_string.cc
)

target_link_libraries(static_string_test
  bela
)

add_executable(iota_test
  iota.cc
)

target_link_libraries(iota_test
  bela
)

add_executable(strings_cat_test
strings_cat.cc
)

target_link_libraries(strings_cat_test
  bela
)