get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

project(${PROJECT_NAME})

set(LIBRARY_NAME ${PROJECT_NAME})

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES source/*.cpp)

add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})

find_package(Vulkan REQUIRED)

target_include_directories(${LIBRARY_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS} include)

target_link_libraries(${APPLICATION_EXE} PUBLIC ${LIBRARY_NAME})

LinkToModule(${LIBRARY_NAME} VulkanInitializer)
