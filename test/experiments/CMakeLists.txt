
add_executable(multiple_windows_exp multiple_windows/main.cpp)

list(APPEND EXPERIMENT_TARGETS_LIST multiple_windows_exp)

# Common steps for all experimental binaries
foreach(target ${EXPERIMENT_TARGETS_LIST})
    set_target_properties(${target} PROPERTIES 
        INSTALL_RPATH "$ORIGIN" 
        BUILD_WITH_INSTALL_RPATH FALSE
    )

    target_compile_definitions(${target} PRIVATE 
        ${DEPENDENCY_DEFINITIONS} 
    )

    target_include_directories(${target} PRIVATE ${PROJECT_INCLUDE_DIR})
    target_link_libraries(${target} ${PROJECT_LINK_LIBS})
endforeach()

# Propagate list to upper cmake directories (InstallRules.cmake)
set(EXPERIMENT_TARGETS_LIST ${EXPERIMENT_TARGETS_LIST} PARENT_SCOPE)