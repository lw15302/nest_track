cmake_minimum_required(VERSION 2.8.12)

set(tracking_project EntranceTracker)

set(src_main_dir ${CMAKE_CURRENT_SOURCE_DIR}/../src/main)
set(src_display_dir ${CMAKE_CURRENT_SOURCE_DIR}/../src/display)
set(src_track_dir ${CMAKE_CURRENT_SOURCE_DIR}/../src/track)

set(include_main_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include/main)
set(include_display_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include/display)
set(include_track_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include/track)

project(${tracking_project})

file(GLOB src_main_out ${src_main_dir}/*.cpp)
file(GLOB src_display_out ${src_display_dir}/*.cpp)
file(GLOB src_track_out ${src_track_dir}/*.cpp)

file(GLOB include_main_out ${include_main_dir}/*.hpp)
file(GLOB include_display_out ${include_display_dir}/*.hpp)
file(GLOB include_track_out ${include_track_dir}/*.hpp)

add_executable(${tracking_project} ${src_main_out} ${src_display_out} ${src_track_out} ${include_main_out} ${include_main_out} ${include_track_out})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ${include_main_dir} ${include_display_dir} ${include_track_dir})

target_link_libraries(${tracking_project} ${OpenCV_LIBS})
