#!/bin/bash

set -e

WORKING_DIRECTORY=`pwd`

mkdir -p src/SDL/build
cd src/SDL/build
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_INSTALL_PREFIX=${WORKING_DIRECTORY}
cmake --build . --config Release
cmake --build . --target install
