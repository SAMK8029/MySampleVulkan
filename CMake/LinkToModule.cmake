
function(LinkToModule TARGET_NAME MODULE_NAME)

    set(MODULE_INCULDE_DIR ${CMAKE_SOURCE_DIR}/Modules/${MODULE_NAME}/include)

    target_include_directories(${TARGET_NAME} PRIVATE ${MODULE_INCULDE_DIR})
    target_link_libraries(${TARGET_NAME} PRIVATE ${MODULE_NAME})

endfunction(LinkToModule)
