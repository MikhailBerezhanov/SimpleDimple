#!/bin/bash

set -e

WORKING_DIRECTORY=`pwd`
PLATFORM="$1"
# Convert to lowercase using bash
PALTFORM="${PLATFORM,,}"

mkdir -p src/SDL/build
cd src/SDL/build
# Clear residue from previous builds
rm -rf ./*

if [[ "$PLATFORM" == "windows" ]]; then
    echo "Configuring SDL2 for Windows with MinGW"
    # Cross-compile for Windows with MinGW
    cmake .. -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
        -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_SYSTEM_PROCESSOR=x86_64 \
        -DCMAKE_INSTALL_PREFIX=${WORKING_DIRECTORY}
else
    echo "Configuring SDL2 for UNIX with Clang"
    # Configure for linux
    cmake .. -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_INSTALL_PREFIX=${WORKING_DIRECTORY}
fi

cmake --build . --config Release
cmake --build . --target install