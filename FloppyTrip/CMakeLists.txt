cmake_minimum_required(VERSION 3.7)
project(floppytrip)

find_package(SDL2 REQUIRED)
find_package(PkgConfig)
pkg_search_module(tinyxml2 REQUIRED tinyxml2)
pkg_search_module(SDL2_image REQUIRED SDL2_image)

include_directories(${SDL2_INCLUDE_DIRS}
        ${tinyxml2_INCLUDE_DIRS}
        ${SDL2_image_INCLUDE_DIRS}
        bass_linux)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O2 -g -march=native -Wall")

set(SOURCE_FILES main.cpp Game.cpp Text.cpp Texture.cpp Vector.cpp Animation.cpp)
add_executable(floppytrip ${SOURCE_FILES})

target_link_libraries(floppytrip
        ${SDL2_LIBRARIES}
        ${tinyxml2_LIBRARIES}
        ${SDL2_image_LIBRARIES}
        ${PROJECT_SOURCE_DIR}/bass_linux/x64/libbass.so
        )