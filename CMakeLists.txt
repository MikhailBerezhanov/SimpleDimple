cmake_minimum_required(VERSION 3.10.0)

project(test_game VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Executable name
set(EXEC_NAME "${CMAKE_PROJECT_NAME}")
# Sources location
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
# Bin/runtime libs destination
set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin")
# Static linkage for stdlib to reduce dependencies
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# List sources
set(SOURCES 
    ${SOURCE_DIR}/main.cpp
)

# Include platform-specific settings
include(PlatformSettings.cmake)

set(SDL_LIB_DIR "${SDL_PREFIX}/lib")
set(SDL_INCLUDE_DIR "${SDL_PREFIX}/include")
set(SDL_LIBS "")

# Collect all SDL libs
foreach(X ${SDL_LIB_NAMES})
    find_library(SDL_EL ${X} PATHS ${SDL_LIB_DIR})
    list(APPEND SDL_LIBS "${SDL_EL}")
    unset(SDL_EL CACHE)
endforeach()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "DEBUG: Configuring for ${CMAKE_SYSTEM_NAME}")
    message(STATUS "DEBUG: Compiler ${CMAKE_CXX_COMPILER}")
    message(STATUS "DEBUG: SDL Libs: ${SDL_EXTRA_LIBS} ${SDL_LIBS}")
    message(STATUS "DEBUG: SDL defs: ${SDL_DEFINITIONS}")
endif()

# Add executable
add_executable(${EXEC_NAME} ${SOURCES})

# Add definitions
target_compile_definitions(${EXEC_NAME} PRIVATE ${SDL_DEFINITIONS})
# Add include dirs
target_include_directories(${EXEC_NAME} PRIVATE ${SDL_INCLUDE_DIR})
# Link libs
target_link_libraries(${EXEC_NAME} ${SDL_EXTRA_LIBS} ${SDL_LIBS})

# set rpath for executable relative to $ORIGIN (the executable location)
# Linux-only, ignored for windows
set_target_properties(${EXEC_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN" 
    BUILD_WITH_INSTALL_RPATH TRUE)

# Include installation rules
include(InstallRules.cmake)


