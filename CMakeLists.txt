cmake_minimum_required(VERSION 3.14)

include(${CMAKE_SOURCE_DIR}/CMake/LinkToModule.cmake)

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

include_directories(Modules/CommonHeaderFiles)

if(WIN32)
    add_definitions(-DWin32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX)
    add_definitions(-DUnix)
    add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
endif()

add_definitions(-DVK_NO_PROTOTYPES)

add_subdirectory(Modules/PresentationEngine)
add_subdirectory(Modules/VulkanInitializer)
add_subdirectory(Modules/RenderingEngine)
