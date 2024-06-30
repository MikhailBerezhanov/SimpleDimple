cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_PROJECT_NAME test_game)

project(${CMAKE_PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SDL_LIB_NAME "libSDL2-2.0.so")
set(SDL_LOCATION "${CMAKE_SOURCE_DIR}/SDL")
set(SDL_LIB_LOCATION "${SDL_LOCATION}/lib")
set(SDL_INCLUDE_LOCATION "${SDL_LOCATION}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_DIR}/main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SDL_INCLUDE_LOCATION})

find_library(SDL_LIB ${SDL_LIB_NAME} PATHS ${SDL_LIB_LOCATION})

message("Lib path ${SDL_LIB}")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${SDL_LIB})

# set rpath for executable to $ORIGIN (look in the same folder as itself)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN/lib" 
    BUILD_WITH_INSTALL_RPATH TRUE)

# install(FILES ${SDL2_LIBS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)

