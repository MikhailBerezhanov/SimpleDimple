cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_PROJECT_NAME test_game)

project(${CMAKE_PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(EXEC_NAME "test_game")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

if(WIN32)
    # For some reason for Windows we need 2 libs
    set(SDL_LIB_NAME "SDL2d" "SDL2maind")
elseif(UNIX)
    set(SDL_LIB_NAME "libSDL2-2.0.so")
endif()

add_executable(${EXEC_NAME} ${SOURCE_DIR}/main.cpp)

target_include_directories(${EXEC_NAME} PRIVATE ${INCLUDE_DIR})

set(SDL_LIB "")

# Collect all libs
foreach(X IN LISTS SDL_LIB_NAME)
    find_library(SDL_EL ${X} PATHS ${LIB_DIR})
    list(APPEND SDL_LIB "${SDL_EL}")
    unset(SDL_EL CACHE)
endforeach()

message("Lib path ${SDL_LIB}")

target_link_libraries(${EXEC_NAME} PRIVATE ${SDL_LIB})

# set rpath for executable to $ORIGIN (look in the same folder as itself)
set_target_properties(${EXEC_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN/../lib" 
    BUILD_WITH_INSTALL_RPATH TRUE)

install(TARGETS ${EXEC_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)

