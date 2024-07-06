#!/bin/bash

set -e

WORKING_DIRECTORY=`pwd`
PLATFORM="$1"
# Convert to lowercase using bash
PALTFORM="${PLATFORM,,}"

build_library() {

    local SRC_DIR="$1"
    
    mkdir -p $SRC_DIR/build
    cd $SRC_DIR/build
    # Clear residue from previous builds
    rm -rf ./*

    if [[ "$PLATFORM" == "windows" ]]; then
        echo "Configuring $SRC_DIR for Windows with MinGW"
        # Cross-compile for Windows with MinGW
        cmake .. -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
            -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
            -DCMAKE_SYSTEM_NAME=Windows \
            -DCMAKE_SYSTEM_PROCESSOR=x86_64 \
            -DCMAKE_INSTALL_PREFIX=${WORKING_DIRECTORY}
    else
        echo "Configuring $SRC_DIR for UNIX with Clang"
        # Configure for linux
        cmake .. -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_C_COMPILER=clang \
            -DCMAKE_SYSTEM_PROCESSOR=x86_64 \
            -DCMAKE_INSTALL_PREFIX=${WORKING_DIRECTORY}
    fi

    cmake --build . --config Release
    cmake --build . --target install
}

# Build SDL
build_library "SDL"
# Build SDL_Image
build_library "SDL_Image"
