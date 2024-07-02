# Base image for msbuild (uses Mono)
# FROM didstopia/msbuild
FROM debian:buster-slim

# Update and install cross-platform build dependencies
# Ignore apt-get weird error related to https absence
RUN apt-get update || true
# Add support for https
# RUN apt-get install -y apt-transport-https || true
RUN apt-get install --no-install-recommends -y \
    clang \
    build-essential \
    xutils-dev \
    libreadline6-dev \
    libncurses5-dev \
    mingw-w64 \
    cmake \
    gdb \
    libx11-dev \
    xorg-dev \
    libgl1-mesa-glx \
    libglib2.0-0

# Install other dependencies
# python-pip git-core
RUN apt-get install --no-install-recommends -y unzip zip curl bash