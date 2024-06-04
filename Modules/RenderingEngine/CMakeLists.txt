get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

project(${PROJECT_NAME})

set(LIBRARY_NAME ${PROJECT_NAME})

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES source/*.cpp)

add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})

find_package(Vulkan REQUIRED)

target_include_directories(${LIBRARY_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS} include)

target_link_libraries(${APPLICATION_EXE} PUBLIC ${LIBRARY_NAME})

file(GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.spv)
file(MAKE_DIRECTORY SHADERS ${CMAKE_BINARY_DIR}/Shaders/)
file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/Shaders)

LinkToModule(${LIBRARY_NAME} VulkanInitializer)
LinkToModule(${LIBRARY_NAME} PresentationEngine)
