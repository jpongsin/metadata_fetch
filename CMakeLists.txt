cmake_minimum_required(VERSION 3.16)
project(metadata_fetch C)

set(CMAKE_C_STANDARD 11)

##dependencies
if(WIN32)
        find_package(FFMPEG REQUIRED)
else()
        find_package(PkgConfig REQUIRED)
        
        pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
                libavcodec libavformat libswscale libavutil)
endif()


add_executable(metadata_fetch src/main.c)

if (WIN32)
        target_include_directories(metadata_fetch PRIVATE ${FFMPEG_INCLUDE_DIRS})
        target_link_libraries(metadata_fetch PRIVATE ${FFMPEG_LIBRARIES})
else()

        target_link_libraries(metadata_fetch PRIVATE PkgConfig::FFMPEG)
        target_link_libraries(metadata_fetch PRIVATE m)
endif()
