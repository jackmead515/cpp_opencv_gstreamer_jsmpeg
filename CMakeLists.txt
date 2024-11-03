cmake_minimum_required(VERSION 2.8)
project( example )

# OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Build the core library
file(GLOB SOURCES
    src/*.h
    src/*.cpp
)
add_executable( example ${SOURCES} )
target_link_libraries( example ${OpenCV_LIBS})