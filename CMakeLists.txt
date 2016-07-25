cmake_minimum_required(VERSION 2.8.12)

set(name EntranceTracker)

set(src_main_dir ${CMAKE_CURRENT_SOURCE_DIR}/src/main)
set(src_display_dir ${CMAKE_CURRENT_SOURCE_DIR}/src/display)

set(include_main_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/main)
set(include_display_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/display)

project(${name})

file(GLOB src_main_out ${src_main_dir}/*.cpp)
file(GLOB src_display_out ${src_display_dir}/*.cpp)

file(GLOB include_main_out ${include_main_dir}/*.hpp)
file(GLOB include_display_out ${include_display_dir}/*.hpp)

add_executable(${name} ${src_main_out} ${src_display_out} ${include_main_out} ${include_main_out})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ${include_main_dir} ${include_display_dir})

target_link_libraries(${name} ${OpenCV_LIBS})
