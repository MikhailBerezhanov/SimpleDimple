
# SDL version to link against
set(SDL_USED_VERSION "SDL2")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(__SDL_PREFIX "/opt/sdl/windows")
    set(__ZLIB_PREFIX "/opt/zlib/windows")
    set(__LIBZIP_PREFIX "/opt/libzip/windows")
    # From sdl2-config --libs
    # !PUT THESE BEFORE SDL2 LIBS!
    set(__SDL_EXTRA_LIBS "-lmingw32" "-mwindows")
    set(__SDL_LIB_NAMES
            "${SDL_USED_VERSION}main"
            "${SDL_USED_VERSION}.dll"
            "${SDL_USED_VERSION}_image.dll"
            "${SDL_USED_VERSION}_ttf.dll")
    # need static pthread for mingw-posix
    set(__OTHER_LIBS "-static" "winpthread")
    set(__ZLIB_LIB_NAMES "zlibstatic")
    set(__LIBZIP_LIB_NAMES "zip")
    # From sdl2-config --cflags
    set(__SDL_DEFINITIONS main=SDL_main)
else()
    # Linux
    set(ENABLE_GTESTS ON)
    set(__SDL_PREFIX "/opt/sdl/linux")
    set(__ZLIB_PREFIX "/opt/zlib/linux")
    set(__LIBZIP_PREFIX "/opt/libzip/linux")
    set(__SDL_LIB_NAMES
            "${SDL_USED_VERSION}"
            "${SDL_USED_VERSION}_image"
            "${SDL_USED_VERSION}_ttf")
    set(__ZLIB_LIB_NAMES "libz.a")
    set(__LIBZIP_LIB_NAMES "zip")
    # From sdl2-config --cflags
    set(__SDL_DEFINITIONS _REENTRANT) 
endif()

# Common function to find libraries
function(find_libs LIB_VAR INC_PATH_VAR LIB_NAMES LIB_PREFIX)
    set(LIBS "")
    # Look for libs
    foreach(LIB ${LIB_NAMES})
        find_library(FOUND_LIB ${LIB} REQUIRED PATHS "${LIB_PREFIX}/lib" NO_DEFAULT_PATH)
        list(APPEND LIBS "${FOUND_LIB}")
        unset(FOUND_LIB CACHE)
    endforeach()
    # Look for includes
    if(NOT EXISTS "${LIB_PREFIX}/include")
        message(FATAL_ERROR "${LIB_PREFIX}/include does not exist")
    endif()
    # Set parent vars
    set(${LIB_VAR} ${LIBS} PARENT_SCOPE)
    set(${INC_PATH_VAR} "${INC_PATH}" PARENT_SCOPE)
endfunction()

find_libs(__SDL_LIBS __SDL_INCLUDE_DIR "${__SDL_LIB_NAMES}" "${__SDL_PREFIX}")
# Not used for now
#find_libs(__ZLIB_LIBS __ZLIB_INCLUDE_DIR "${__ZLIB_LIB_NAMES}" "${__ZLIB_PREFIX}")
#find_libs(__LIBZIP_LIBS __LIBZIP_INCLUDE_DIR "${__LIBZIP_LIB_NAMES}" "${__LIBZIP_PREFIX}")

# A full collection of libs
set(DEPENDENCY_LIBS 
    ${__SDL_EXTRA_LIBS} 
    ${__SDL_LIBS}
    ${__LIBZIP_LIBS}
    ${__ZLIB_LIBS}
    ${__OTHER_LIBS}
)

# A full collection of includes
set(DEPENDENCY_INCLUDE_DIRS
    ${__SDL_INCLUDE_DIR}
    ${__LIBZIP_INCLUDE_DIR}
    ${__ZLIB_INCLUDE_DIR}
)

# A full list of compile definitions
set(DEPENDENCY_DEFINITIONS
    ${__SDL_DEFINITIONS}
)

set(SDL_LIB_DIR "${__SDL_PREFIX}/lib")
set(SDL_BIN_DIR "${__SDL_PREFIX}/bin")

# Exported variables:
# SDL_USED_VERSION
# SDL_LIB_DIR
# SDL_BIN_DIR
# DEPENDENCY_LIBS
# DEPENDENCY_INCLUDE_DIRS
# DEPENDENCY_DEFINITIONS
# ENABLE_GTESTS
