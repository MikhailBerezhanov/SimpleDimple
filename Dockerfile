# Base image for msbuild (uses Mono)
# TODO: so far the lib and exe are built successfully, but don't work
# The app just hangs there without any activity
# If built on host machine (Ubuntu 22.04) everything works as expected
# The image might be too old, try something newer?
FROM didstopia/msbuild

# Working directory
ARG WORKING_DIRECTORY="/workspaces/SimpleDimple"

# Update and install cross-platform build dependencies
# Ignore apt-get weird error related to https absence
RUN apt-get update || true
# Add support for https
RUN apt-get install -y apt-transport-https
RUN apt-get install -y \
    clang \
    build-essential \
    xutils-dev \
    # libsdl2-2.0-0 \
    # libsdl2-dev \
    # libsdl2-image-2.0-0 \
    # libsdl2-image-dev \
    libreadline6-dev \
    libncurses5-dev \
    mingw-w64 \
    cmake \
    gdb

# Install other dependencies
# python-pip git-core
RUN apt-get install -y unzip zip curl bash
# Use this folder for the project 
RUN mkdir -p ${WORKING_DIRECTORY}
WORKDIR ${WORKING_DIRECTORY}