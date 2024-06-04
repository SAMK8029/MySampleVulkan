get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

project(${PROJECT_NAME})

set(LIBRARY_NAME ${PROJECT_NAME})

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES source/*.cpp)

add_library(${LIBRARY_NAME} STATIC ${HEADERS} ${SOURCES})

find_package(Vulkan)

target_include_directories(${LIBRARY_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS} include)
target_link_libraries(${APPLICATION_EXE} PUBLIC ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Dependency/GLFW/include)

if(WIN32)
    target_link_libraries(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Dependency/GLFW/lib/Windows/libglfw3.a)
elseif(UNIX)
    target_link_libraries(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Dependency/GLFW/lib/Linux/libglfw3.a)
endif()

LinkToModule(${LIBRARY_NAME} VulkanInitializer)
LinkToModule(${LIBRARY_NAME} RenderingEngine)
