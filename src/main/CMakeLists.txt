aux_source_directory(. SRCS)
add_executable(liveStream ${SRCS})

target_link_libraries(liveStream base)