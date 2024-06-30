cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_PROJECT_NAME test_game)

project(${CMAKE_PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(ExternalProject)

ExternalProject_Add(
    SDL2
    PREFIX ${CMAKE_SOURCE_DIR}/SDL
    TMP_DIR ${CMAKE_SOURCE_DIR}/tmp
    STAMP_DIR ${CMAKE_SOURCE_DIR}/stamp
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/SDL
    BINARY_DIR ${CMAKE_SOURCE_DIR}/SDL/build
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang
    INSTALL_COMMAND ""
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/SDL/include)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/SDL/build/include)

ExternalProject_Get_Property(SDL2 BINARY_DIR)

set(SDL2_LIBS 
    ${BINARY_DIR}/libSDL2-2.0.so 
    ${BINARY_DIR}/libSDL2-2.0.so.0 
    ${BINARY_DIR}/libSDL2-2.0.so.0.3100.0
)

find_library(SDL2_LIB libSDL2-2.0.so PATHS ${BINARY_DIR})

message("Lib path ${SDL2_LIB}")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_LIB})

# set rpath for executable to $ORIGIN (look in the same folder as itself)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN/lib" 
    BUILD_WITH_INSTALL_RPATH TRUE)


install(FILES ${SDL2_LIBS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)