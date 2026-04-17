cmake_minimum_required(VERSION 3.16)
project(metadata_fetch C)

set(CMAKE_C_STANDARD 11)

##dependencies
find_package(PkgConfig REQUIRED)

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
        libavcodec libavformat libswscale libavutil)

add_executable(metadata_fetch src/main.c)

target_link_libraries(metadata_fetch PRIVATE PkgConfig::FFMPEG)

target_link_libraries(metadata_fetch PRIVATE m)