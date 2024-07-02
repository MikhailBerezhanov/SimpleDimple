cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_PROJECT_NAME test_game)

project(${CMAKE_PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

# Set Compiler (in cli)
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)

# Set to "Windows" to force compile for Windows
# set(CMAKE_SYSTEM_NAME "Windows")
# Executable name
set(EXEC_NAME "test_game")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")

message("Configuring for ${CMAKE_SYSTEM_NAME}")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # On Windows, these 2 libraries are so-called "import libraries"
    # We need them to link against SDL2.dll located in the bin/ folder
    # So we need to use these 2 .libs to link that SDL2.dll
    # And then we need to ensure that the SDL2.dll is available at runtime 
    if(${CMAKE_CXX_COMPILER} STREQUAL "clang++")
        # clang
        set(SDL_LIB_NAME "SDL2d" "SDL2maind")
    else()
        # mingw
        set(SDL_LIB_NAME "libSDL2main.a" "libSDL2.dll.a")
    endif()
else()
    # Unix
    set(SDL_LIB_NAME "libSDL2.so")
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

target_link_libraries(${EXEC_NAME} ${SDL_LIB})

# set rpath for executable to $ORIGIN (look in the same folder as itself)
set_target_properties(${EXEC_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN/../lib" 
    BUILD_WITH_INSTALL_RPATH TRUE)

install(TARGETS ${EXEC_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)


