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
# -v $(pwd):/workspace : map current directory into /workspace
# -w /workspace : set container WORKDIR
# $CONTAINER : image to use
# /bin/bash -c "./build_sdl.sh" : run script at startup
docker run -it --rm \
    --name "$CONTAINER-instance" \
    -v $(pwd):/workspace \
    -w /workspace \
    "$CONTAINER" \
    /bin/bash -c "./build_sdl.sh $PLATFORM ; exec /bin/bash"
    