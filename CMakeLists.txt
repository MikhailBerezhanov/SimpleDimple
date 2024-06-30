cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_PROJECT_NAME test_game)

project(${CMAKE_PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXEC_NAME "test_game")
set(SDL_LIB_NAME "libSDL2-2.0.so")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

add_executable(${EXEC_NAME} ${SOURCE_DIR}/main.cpp)

target_include_directories(${EXEC_NAME} PRIVATE ${INCLUDE_DIR})

find_library(SDL_LIB ${SDL_LIB_NAME} PATHS ${LIB_DIR})

message("Lib path ${SDL_LIB}")

target_link_libraries(${EXEC_NAME} PRIVATE ${SDL_LIB})

# set rpath for executable to $ORIGIN (look in the same folder as itself)
set_target_properties(${EXEC_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN/../lib" 
    BUILD_WITH_INSTALL_RPATH TRUE)

install(TARGETS ${EXEC_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)

