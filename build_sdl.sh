#!/bin/bash

WORKING_DIRECTORY="."

mkdir -p src/SDL/build && \
    cd src/SDL/build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=${WORKING_DIRECTORY}/SDL && \
    cmake --build . --config Release && \
    cmake --build . --target install
