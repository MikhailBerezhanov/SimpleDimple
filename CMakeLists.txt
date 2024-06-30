cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_PROJECT_NAME test_game)

project(${CMAKE_PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SDL2 library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} -lSDL2)

# set rpath for executable to $ORIGIN (look in the same folder as itself)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN/lib" 
    BUILD_WITH_INSTALL_RPATH TRUE)


# install(FILES ${LIB_DEPENDENCIES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)