aux_source_directory(. SRCS)
add_executable(liveStream ${SRCS})


# add_dependencies(base jsoncpp)  # 添加依赖
target_link_libraries(liveStream base network mmedia live jsoncpp.a crypto)