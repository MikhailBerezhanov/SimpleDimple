if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(SDL_PREFIX "/SDL/Windows")
    # On Windows, these 2 libraries are so-called "import libraries"
    # We need them to link against SDL2.dll located in the bin/ folder
    # So we need to use these 2 .libs to link that SDL2.dll
    # And then we need to ensure that the SDL2.dll is available at runtime 
    if(${CMAKE_CXX_COMPILER} MATCHES "cl.exe")
        # clang
        set(SDL_LIB_NAMES "SDL2maind" "SDL2d")
    else()
        # mingw 
        # From sdl2-config --libs
        # !PUT THESE BEFORE SDL2 LIBS!
        set(SDL_EXTRA_LIBS "-lmingw32" "-mwindows")
        # libSDL2main.a libSDL2.dll.a
        set(SDL_LIB_NAMES "SDL2main" "SDL2.dll")
    endif()
    # From sdl2-config --cflags
    set(SDL_DEFINITIONS main=SDL_main)
else()
    # Unix
    set(SDL_PREFIX "/SDL/Unix")
    # libSDL2.so
    set(SDL_LIB_NAMES "SDL2") 
    # From sdl2-config --cflags
    set(SDL_DEFINITIONS _REENTRANT) 
endif()
