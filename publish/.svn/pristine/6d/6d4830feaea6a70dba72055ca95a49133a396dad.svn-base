
cmake_minimum_required (VERSION 2.8)

project(publish_rtmp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt -lpthread")

SET(CMAKE_BUILD_TYPE "Debug")

include_directories(${PROJECT_SOURCE_DIR}/common)
#include_directories(${PROJECT_SOURCE_DIR}/librtmp)
include_directories(${PROJECT_SOURCE_DIR}/packdeff)
include_directories(${PROJECT_SOURCE_DIR}/rtmplib)
include_directories(~/usr/local/include)

#link_directories(${PROJECT_SOURCE_DIR}/librtmp)
link_directories(${PROJECT_SOURCE_DIR}/rtmplib)
link_directories(~/usr/local/lib)


aux_source_directory(. DIR_SRC)
aux_source_directory(./common DIR_SRC)
aux_source_directory(./packdeff DIR_SRC)
#aux_source_directory(./librtmp DIR_SRC)
#aux_source_directory(./rtmp-lib DIR_SRC)

add_executable(publish_rtmp ${DIR_SRC})

#/target_link_libraries(publish_rtmp rtmp)/
target_link_libraries(publish_rtmp librtmp-asyn.a)
target_link_libraries(publish_rtmp librtmpClient.a)
target_link_libraries(publish_rtmp librtmp.a)
target_link_libraries(publish_rtmp libEpollClientR_64.a)
#target_link_libraries(publish_rtmp gcov)
target_link_libraries(publish_rtmp libevent.a)
target_link_libraries(publish_rtmp libevent_core.a)
target_link_libraries(publish_rtmp libevent_extra.a)
target_link_libraries(publish_rtmp libevent_pthreads.a)
target_link_libraries(publish_rtmp ssl)
target_link_libraries(publish_rtmp crypto)
#target_link_libraries(publish_rtmp libtcmalloc.a)
target_link_libraries(publish_rtmp libjemalloc.a)
target_link_libraries(publish_rtmp pthread)
