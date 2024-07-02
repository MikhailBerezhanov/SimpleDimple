#!/bin/bash

# An attempt to reproduce VSCode's Dev Containers extension behaviour

set -e

# Container name
CONTAINER="sdl-container"
# Script args
PLATFORM="$1"

# Build image
docker build -t "$CONTAINER" .

# Run container
# -it --rm : interactive mode, remove container after it's stopped
# --name "$CONTAINER-instance" : container name
# -e DISPLAY=$DISPLAY : pass X11 display into docker
# -v /tmp/.X11-unix:/tmp/.X11-unix : map X11 folder
# -v $(pwd):/workspace : map current directory into /workspace
# -w /workspace : set container WORKDIR
# $CONTAINER : image to use
# /bin/bash -c "./build_sdl.sh $PLATFORM; /bin/bash" : run script at startup
docker run -it --rm \
    --name "$CONTAINER-instance" \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v $(pwd):/workspace \
    -w /workspace \
    "$CONTAINER" \
    /bin/bash -c "./build_sdl.sh $PLATFORM; /bin/bash"


    