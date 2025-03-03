cmake_minimum_required (VERSION 3.8)

# set solution name, version & language
project ("BSPDemo" VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# set source files
file(GLOB SOURCES "src/*.cpp")

# WIN32: set WinMain as entry point of BSPDemo
add_executable(BSPDemo WIN32 ${SOURCES})

# set using UNICODE characterset
add_definitions(-DUNICODE -D_UNICODE)
