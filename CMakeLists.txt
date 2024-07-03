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

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Static linkage for stdlib to reduce dependencies
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

message("Configuring for ${CMAKE_SYSTEM_NAME}")
message("Compiler ${CMAKE_CXX_COMPILER}")

# Add executable
add_executable(${EXEC_NAME} ${SOURCE_DIR}/main.cpp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # On Windows, these 2 libraries are so-called "import libraries"
    # We need them to link against SDL2.dll located in the bin/ folder
    # So we need to use these 2 .libs to link that SDL2.dll
    # And then we need to ensure that the SDL2.dll is available at runtime 
    if(${CMAKE_CXX_COMPILER} MATCHES "cl.exe")
        # clang
        set(SDL_LIB_NAMES "SDL2d" "SDL2maind")
    else()
        # mingw 
        # From sdl2-config --libs
        # !PUT THESE BEFORE SDL2 LIBS!
        set(SDL_EXTRA_LIBS "-lmingw32" "-mwindows")
        # libSDL2main.a libSDL2.dll.a
        set(SDL_LIB_NAMES "SDL2main" "SDL2.dll")
    endif()
    set(SDL_PREFIX "/SDL/Windows")
    # From sdl2-config --cflags
    set(SDL_DEFINITIONS main=SDL_main)
else()
    # Unix
    set(SDL_PREFIX "/SDL/Unix")
    set(SDL_EXTRA_LIBS "")
    # libSDL2.so
    set(SDL_LIB_NAMES "SDL2") 
    # From sdl2-config --cflags
    set(SDL_DEFINITIONS _REENTRANT) 
endif()

set(SDL_LIB_DIR "${SDL_PREFIX}/lib")
set(SDL_INCLUDE_DIR "${SDL_PREFIX}/include")
set(SDL_LIBS "")

# Collect all libs
foreach(X IN LISTS SDL_LIB_NAMES)
    find_library(SDL_EL ${X} PATHS ${SDL_LIB_DIR})
    list(APPEND SDL_LIBS "${SDL_EL}")
    unset(SDL_EL CACHE)
endforeach()

message("SDL Libs: ${SDL_EXTRA_LIBS} ${SDL_LIBS}")
message("SDL defs: ${SDL_DEFINITIONS}")

# Add definitions
target_compile_definitions(${EXEC_NAME} PRIVATE ${SDL_DEFINITIONS})
# Add include dirs
target_include_directories(${EXEC_NAME} PRIVATE ${SDL_INCLUDE_DIR})
# Link libs
target_link_libraries(${EXEC_NAME} ${SDL_EXTRA_LIBS} ${SDL_LIBS})

# set rpath for executable relative to $ORIGIN (the executable location)
set_target_properties(${EXEC_NAME} PROPERTIES 
    INSTALL_RPATH "$ORIGIN" 
    BUILD_WITH_INSTALL_RPATH TRUE)

# Installation
install(TARGETS ${EXEC_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
# Installation - windows-specific
    install(FILES ${SDL_PREFIX}/bin/SDL2.dll 
        DESTINATION ${CMAKE_SOURCE_DIR}/bin)
else()
# Installation - linux-specific
    install(DIRECTORY ${SDL_PREFIX}/lib/ 
        DESTINATION ${CMAKE_SOURCE_DIR}/bin
        FILES_MATCHING PATTERN "*.so" PATTERN "*.so*"
        PATTERN "cmake" EXCLUDE
        PATTERN "pkgconfig" EXCLUDE)
endif()


