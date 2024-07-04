if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(SDL_PREFIX "/SDL/Windows")
    set(ZLIB_PREFIX "/ZLIB/Windows")
    # On Windows, these 2 libraries are so-called "import libraries"
    # We need them to link against SDL2.dll located in the bin/ folder
    # So we need to use these 2 .libs to link that SDL2.dll
    # And then we need to ensure that the SDL2.dll is available at runtime 
    if(${CMAKE_CXX_COMPILER} MATCHES "cl.exe")
        # clang (todo: SDL2_image ?)
        set(__SDL_LIB_NAMES "SDL2maind" "SDL2d")
    else()
        # mingw 
        # From sdl2-config --libs
        # !PUT THESE BEFORE SDL2 LIBS!
        set(SDL_EXTRA_LIBS "-lmingw32" "-mwindows")
        # libSDL2main.a libSDL2.dll.a
        set(__SDL_LIB_NAMES "SDL2main" "SDL2.dll" "SDL2_image.dll")
    endif()
    set(__ZLIB_LIB_NAMES "zlibstatic")
    # From sdl2-config --cflags
    set(SDL_DEFINITIONS main=SDL_main)
else()
    # Unix
    set(SDL_PREFIX "/SDL/Unix")
    set(ZLIB_PREFIX "/ZLIB/Unix")
    # libSDL2.so
    set(__SDL_LIB_NAMES "SDL2" "SDL2_image") 
    set(__ZLIB_LIB_NAMES "libz.a")
    # From sdl2-config --cflags
    set(SDL_DEFINITIONS _REENTRANT) 
endif()

set(SDL_LIB_DIR "${SDL_PREFIX}/lib")
set(SDL_BIN_DIR "${SDL_PREFIX}/bin")
set(SDL_INCLUDE_DIR "${SDL_PREFIX}/include")

set(ZLIB_INCLUDE_DIR "${ZLIB_PREFIX}/include")
set(ZLIB_LIB_DIR "${ZLIB_PREFIX}/lib")

set(SDL_LIBS "")

# Collect all SDL libs
foreach(X ${__SDL_LIB_NAMES})
    find_library(SDL_EL ${X} PATHS ${SDL_LIB_DIR} NO_DEFAULT_PATH)
    list(APPEND SDL_LIBS "${SDL_EL}")
    unset(SDL_EL CACHE)
endforeach()

find_library(ZLIB_LIBS ${__ZLIB_LIB_NAMES} PATHS ${ZLIB_LIB_DIR} NO_DEFAULT_PATH)

# Exported variables:
# SDL_PREFIX
# SDL_DEFINITIONS
# SDL_LIBS
# SDL_LIB_DIR
# SDL_BIN_DIR
# SDL_INCLUDE_DIR
