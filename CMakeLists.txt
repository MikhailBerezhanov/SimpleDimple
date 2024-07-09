cmake_minimum_required(VERSION 3.16.0)

project(test_game VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Executable name
set(EXEC_NAME ${CMAKE_PROJECT_NAME})
# Config header location
set(CONFIG_DIR ${CMAKE_SOURCE_DIR}/config)
# CMake modules location
set(MODULES_DIR ${CMAKE_SOURCE_DIR}/cmake)
# Sources location
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
# Include location
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
# Bin/runtime libs destination
set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/out)
# Tests location
set(TESTS_DIR ${CMAKE_SOURCE_DIR}/test)
# Assets to be installed location ( !ASSETS_DIR already reserved in RuntimeConfigs.cmake! )
set(ASSETS_TO_INSTALL_DIR ${CMAKE_SOURCE_DIR}/assets)
# Static linkage for stdlib to reduce dependencies
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# List sources (except for main.cpp)
set(SOURCES_NO_MAIN 
    ${SOURCE_DIR}/sdl_window.cpp
    ${SOURCE_DIR}/sdl_renderer.cpp
    ${SOURCE_DIR}/sdl_texture.cpp
    ${SOURCE_DIR}/sdl_surface.cpp
)
# Full list of sources
set(SOURCES 
    ${SOURCES_NO_MAIN}
    ${SOURCE_DIR}/main.cpp
)

# Include runtime configs (LOGS_DIR, ASSETS_DIR,...)
include(${MODULES_DIR}/RuntimeConfigs.cmake)

# Include platform-specific settings (SDL_PREFIX, SDL_LIB_DIR, SDL_LIB_NAMES, ...)
include(${MODULES_DIR}/PlatformDependencies.cmake)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "DEBUG: Configuring for ${CMAKE_SYSTEM_NAME}")
    message(STATUS "DEBUG: Compiler ${CMAKE_CXX_COMPILER}")
    message(STATUS "DEBUG: Dependency Libs: ${DEPENDENCY_LIBS}")
    message(STATUS "DEBUG: Dependency definitions ${DEPENDENCY_DEFINITIONS}")
    message(STATUS "DEBUG: Dependency includes ${DEPENDENCY_INCLUDE_DIRS}")
endif()

# Add executable
add_executable(${EXEC_NAME} ${SOURCES})

# Generate config.h from config.h.in
configure_file(${CONFIG_DIR}/config.h.in ${INCLUDE_DIR}/config.h @ONLY)
# Generate sdl.h from sdl.h.in
configure_file(${CONFIG_DIR}/sdl.h.in ${INCLUDE_DIR}/sdl.h @ONLY)

# Add definitions
target_compile_definitions(${EXEC_NAME} PRIVATE 
    ${DEPENDENCY_DEFINITIONS} 
)

# Add include dirs
target_include_directories(${EXEC_NAME} PRIVATE 
    ${DEPENDENCY_INCLUDE_DIRS}
    ${INCLUDE_DIR}
)
# Link libs (ZLIB after LIBZIP)
target_link_libraries(${EXEC_NAME} 
    ${DEPENDENCY_LIBS} 
)

# set rpath for executable relative to $ORIGIN (the executable location)
# Linux-only, ignored for windows
set_target_properties(${EXEC_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN" 
    BUILD_WITH_INSTALL_RPATH FALSE)

# No testing for Windows version as there's no way we can run .exe in linux container
if (ENABLE_GTESTS)
    # Enable tests
    enable_testing()
    # Specify sources for tests
    set(GTEST_SOURCES ${SOURCES_NO_MAIN})
    # Specify include path for tests
    set(GTEST_INCLUDE_DIR 
        ${DEPENDENCY_INCLUDE_DIRS} 
        ${INCLUDE_DIR}
    )
    # Specify link libs
    set(GTEST_LINK_LIBS ${DEPENDENCY_LIBS})
    # Do not automatically install gtest
    set(INSTALL_GTEST OFF)
    # Add subdir with tests
    add_subdirectory(${TESTS_DIR}/utest)
endif()

# Include installation rules
include(${MODULES_DIR}/InstallRules.cmake)


