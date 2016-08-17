cmake_minimum_required(VERSION 3.2.2)

set(tracking_project EntranceTracker)

set(src_main_dir ${CMAKE_SOURCE_DIR}/host/src/main)
set(src_display_dir ${CMAKE_SOURCE_DIR}/tracking/src/display)
set(src_server_dir ${CMAKE_SOURCE_DIR}/host/src/server)
set(src_track_dir ${CMAKE_SOURCE_DIR}/tracking/src/track)

set(include_display_dir ${CMAKE_SOURCE_DIR}/tracking/include/display)
set(include_server_dir ${CMAKE_SOURCE_DIR}/host/include/include)
set(include_track_dir ${CMAKE_SOURCE_DIR}/tracking/include/track)

project(${tracking_project})

file(GLOB src_main_out ${src_main_dir}/*.cpp)
file(GLOB src_display_out ${src_display_dir}/*.cpp)
file(GLOB src_track_out ${src_track_dir}/*.cpp)
file(GLOB src_server_out ${src_server_dir}/*.cpp)

file(GLOB include_display_out ${include_display_dir}/*.hpp)
file(GLOB include_track_out ${include_track_dir}/*.hpp)
file(GLOB include_server_out ${include_server_dir}/*.hpp)


add_executable(${tracking_project} ${src_main_out} ${src_display_out} ${src_track_out} ${src_server_out} ${include_track_out} ${include_display_out} ${include_server_out})

set_property(TARGET ${tracking_project} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${tracking_project} PROPERTY CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ${include_display_dir} ${include_track_dir} ${include_server_out})

target_link_libraries(${tracking_project} ${OpenCV_LIBS} -pthread)
