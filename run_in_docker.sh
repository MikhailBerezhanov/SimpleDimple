#!/bin/bash

# An attempt to reproduce VSCode's Dev Containers extension behaviour

set -e

# Container name
CONTAINER="sdl-container"

# Build image
docker build -t "$CONTAINER" .

# Allow docker to use X11
xhost +local:docker

# Run container
# -it --rm : interactive mode, remove container after it's stopped
# --name "$CONTAINER-instance" : container name
# -e DISPLAY=$DISPLAY : pass X11 display into docker
# -v /tmp/.X11-unix:/tmp/.X11-unix : map X11 folder
# -v $(pwd):/workspace : map current directory into /workspace
# -w /workspace : set container WORKDIR
# $CONTAINER : image to use
docker run -it --rm \
    --name "$CONTAINER-instance" \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v $(pwd):/workspace \
    -w /workspace \
    "$CONTAINER" \
    /bin/bash


    