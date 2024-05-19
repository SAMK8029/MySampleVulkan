cmake_minimum_required(VERSION 3.14)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.14)

project(MySampleVulkan3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(APPLICATION_EXE ${PROJECT_NAME})

add_executable(${APPLICATION_EXE} main.cpp)

add_subdirectory(Modules/RenderingEngine)

if(WIN32)
    add_definitions(-DWin32)
elseif(UNIX)
    add_definitions(-DUnix)
endif()
