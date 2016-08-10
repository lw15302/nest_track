cmake_minimum_required(VERSION 2.8.12)

set(tracking_project EntranceTracker)

set(src_display_dir ${CMAKE_CURRENT_SOURCE_DIR}/../tracking/src/display)
set(src_track_dir ${CMAKE_CURRENT_SOURCE_DIR}/../tracking/src/track)
set(src_server_dir ${CMAKE_CURRENT_SOURCE_DIR}/../host/src/)

set(include_display_dir ${CMAKE_CURRENT_SOURCE_DIR}/../tracking/include/display)
set(include_track_dir ${CMAKE_CURRENT_SOURCE_DIR}/../tracking/include/track)
set(include_server_dir) ${CMAKE_CURRENT_SOURCE_DIR}/../host/include)

project(${tracking_project})

file(GLOB src_display_out ${src_display_dir}/*.cpp)
file(GLOB src_track_out ${src_track_dir}/*.cpp)
file(GLOB src_server_out ${src_server_dir}/*.c)

file(GLOB include_display_out ${include_display_dir}/*.hpp)
file(GLOB include_track_out ${include_track_dir}/*.hpp)
file(GLOB include_server_out ${include_server_dir}/*.h)

add_executable(${tracking_project} ${src_display_out} ${src_track_out} ${src_server_out} ${include_track_out} ${include_display_out} ${include_server_out})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ${include_display_dir} ${include_track_dir} ${include_server_out})

target_link_libraries(${tracking_project} ${OpenCV_LIBS})
