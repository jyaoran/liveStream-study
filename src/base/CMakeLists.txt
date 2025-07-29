aux_source_directory(. DIR_LIB_SRCS)
add_library(base ${DIR_LIB_SRCS})

add_subdirectory(tests)
# add_executable(base_test ${DIR_LIB_SRCS})
# target_link_libraries(base base jsoncpp.a) # 链接库base  