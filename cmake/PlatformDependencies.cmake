
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(__SDL_PREFIX "/opt/sdl/windows")
    set(__ZLIB_PREFIX "/opt/zlib/windows")
    set(__LIBZIP_PREFIX "/opt/libzip/windows")
    # From sdl2-config --libs
    # !PUT THESE BEFORE SDL2 LIBS!
    set(__SDL_EXTRA_LIBS "-lmingw32" "-mwindows")
    set(__SDL_LIB_NAMES "SDL2main" "SDL2.dll" "SDL2_image.dll")
    # need static pthread for mingw-posix
    set(__OTHER_LIBS "-static" "winpthread")
    set(__ZLIB_LIB_NAMES "zlibstatic")
    set(__LIBZIP_LIB_NAMES "zip")
    # From sdl2-config --cflags
    set(__SDL_DEFINITIONS main=SDL_main)
else()
    # Linux
    set(ENABLE_GTESTS TRUE)
    set(__SDL_PREFIX "/opt/sdl/linux")
    set(__ZLIB_PREFIX "/opt/zlib/linux")
    set(__LIBZIP_PREFIX "/opt/libzip/linux")
    set(__SDL_LIB_NAMES "SDL2" "SDL2_image") 
    set(__ZLIB_LIB_NAMES "libz.a")
    set(__LIBZIP_LIB_NAMES "zip")
    # From sdl2-config --cflags
    set(__SDL_DEFINITIONS _REENTRANT) 
endif()

set(SDL_LIB_DIR "${__SDL_PREFIX}/lib")
set(SDL_BIN_DIR "${__SDL_PREFIX}/bin")

set(__SDL_INCLUDE_DIR "${__SDL_PREFIX}/include")

set(__ZLIB_LIB_DIR "${__ZLIB_PREFIX}/lib")
set(__ZLIB_INCLUDE_DIR "${__ZLIB_PREFIX}/include")

set(__LIBZIP_LIB_DIR "${__LIBZIP_PREFIX}/lib")
set(__LIBZIP_INCLUDE_DIR "${__LIBZIP_PREFIX}/include")

set(__SDL_LIBS "")

# Collect all SDL libs
foreach(X ${__SDL_LIB_NAMES})
    find_library(__SDL_EL ${X} PATHS ${SDL_LIB_DIR} NO_DEFAULT_PATH)
    list(APPEND __SDL_LIBS "${__SDL_EL}")
    unset(__SDL_EL CACHE)
endforeach()

# Not used for now
# find_library(__ZLIB_LIBS ${__ZLIB_LIB_NAMES} PATHS ${__ZLIB_LIB_DIR} NO_DEFAULT_PATH)
# find_library(__LIBZIP_LIBS ${__LIBZIP_LIB_NAMES} PATHS ${__LIBZIP_LIB_DIR} NO_DEFAULT_PATH)

# A full collection of libs
set(DEPENDENCY_LIBS 
    ${__SDL_EXTRA_LIBS} 
    ${__SDL_LIBS}
    # ${__LIBZIP_LIBS}
    # ${__ZLIB_LIBS}
    ${__OTHER_LIBS}
)

# A full collection of includes
set(DEPENDENCY_INCLUDE_DIRS
    ${__SDL_INCLUDE_DIR} 
    # ${__LIBZIP_INCLUDE_DIR}
)

# A full list of compile definitions
set(DEPENDENCY_DEFINITIONS
    ${__SDL_DEFINITIONS}
)

# Exported variables:
# SDL_LIB_DIR
# SDL_BIN_DIR
# DEPENDENCY_LIBS
# DEPENDENCY_INCLUDE_DIRS
# DEPENDENCY_DEFINITIONS
# ENABLE_GTESTS
