CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(liveStream)

SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)

# SET(CMAKE_CXX_STANDARD 11)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 \
        -Wno-unused-variable -lrt -pthread")
elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Release mode")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 \
        -Wno-unused-variable -lrt -pthread")
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -Wno-error=shadow \
    -Wno-sign-compare")

message(STATUS "USING GCC VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.2.9)
    message(STATUS "GCC Version > 7.3.0, USING -Wno-error=unused-result -Werror=return-type")
    set(CMAKE_CXX_FLAGS "-Wno-error=unused-result -Werror=return-type ${CMAKE_CXX_FLAGS}")
endif()

# Add subdirectories
include_directories(src)
add_subdirectory(src)