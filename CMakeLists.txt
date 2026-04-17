cmake_minimum_required(VERSION 3.16)
project(metadata_fetch C)

set(CMAKE_C_STANDARD 11)

##dependencies
if(WIN32)
    include_directories("${FFMPEG_ROOT}/include")
    link_directories("${FFMPEG_ROOT}/lib")
    set(FFMPEG_LIBS avcodec avformat swscale avutil)
else()
        find_package(PkgConfig REQUIRED)
        
        pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
                libavcodec libavformat libswscale libavutil)
        set(FFMPEG_LIBS PkgConfig::FFMPEG)
endif()


add_executable(metadata_fetch src/main.c)
target_link_libraries(metadata_fetch PRIVATE ${FFMPEG_LIBS})

if (NOT WIN32)
        target_link_libraries(metadata_fetch PRIVATE m)
endif()
