# Base image for msbuild (uses Mono)
# FROM didstopia/msbuild
FROM ubuntu

# Working directory
ARG WORKING_DIRECTORY="/workspaces/SimpleDimple"

# Update and install cross-platform build dependencies
# Ignore apt-get weird error related to https absence
RUN apt-get update || true
# Add support for https
RUN apt-get install -y apt-transport-https || true
RUN apt-get install -y \
    clang \
    build-essential \
    xutils-dev \
    libreadline6-dev \
    libncurses5-dev \
    mingw-w64 \
    cmake \
    gdb

# Install other dependencies
# python-pip git-core
RUN apt-get install -y unzip zip curl bash