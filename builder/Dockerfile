#
#   This dockerfile is to be used only to create a working image with SDL support
#

FROM debian:buster-slim

#### Dependencies ####

RUN apt-get update || true

RUN apt-get install -y \
    clang \
    build-essential \
    xutils-dev \
    libreadline6-dev \
    libncurses5-dev \
    mingw-w64 \
    gdb \
    libx11-dev \
    xorg-dev \
    libgl1-mesa-glx \
    libglib2.0-0

# For SDL_Image (Linux only, for MinGW there's no Windows substitutes)
# TODO: Do something with that
RUN apt-get install -y \
    libpng-dev \
    libjpeg-dev \
    libtiff-dev \
    libwebp-dev

# Install other dependencies
# python-pip git-core
RUN apt-get install -y \
    unzip \
    zip \
    curl \
    bash \
    procps

# Install CMake 3.21
ADD https://cmake.org/files/v3.21/cmake-3.21.0-linux-x86_64.tar.gz /tmp
RUN tar -xzf /tmp/cmake-3.21.0-linux-x86_64.tar.gz -C /usr/local --strip-components=1 --no-same-owner 
RUN rm /tmp/cmake-3.21.0-linux-x86_64.tar.gz

#### Build from sources ####

# Create Unix/Windows directories for SDL libs and includes
ENV SDL_PREFIX_UNIX=/opt/sdl/unix
ENV SDL_PREFIX_WINDOWS=/opt/sdl/windows

ENV ZLIB_PREFIX_UNIX=/opt/zlib/unix
ENV ZLIB_PREFIX_WINDOWS=/opt/zlib/windows

ENV LIBZIP_PREFIX_UNIX=/opt/libzip/unix
ENV LIBZIP_PREFIX_WINDOWS=/opt/libzip/windows

ARG SDL_SRC=/tmp/SDL_SOURCES
ARG SDL_IMAGE_SRC=/tmp/SDL_IMAGE_SOURCES
ARG ZLIB_SRC=/tmp/ZLIB_SOURCES
ARG LIBZIP_SRC=/tmp/LIBZIP_SOURCES

ARG SDL_ARCHIVE=/tmp/SDL.tar.gz
ARG SDL_IMAGE_ARCHIVE=/tmp/SDL_Image.tar.gz
ARG ZLIB_ARCHIVE=/tmp/ZLIB.tar.gz
ARG LIBZIP_ARCHIVE=/tmp/LIBZIP.tar.gz

# Download SDL sources
ADD https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.30.4.tar.gz ${SDL_ARCHIVE}
# Download SDL_Image sources
ADD https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.8.2.tar.gz ${SDL_IMAGE_ARCHIVE}
# Download ZLIB sources
ADD https://github.com/madler/zlib/archive/refs/tags/v1.3.1.tar.gz ${ZLIB_ARCHIVE}
# Download libzip sources
ADD https://github.com/nih-at/libzip/archive/refs/tags/v1.10.1.tar.gz ${LIBZIP_ARCHIVE}

RUN mkdir -p ${SDL_PREFIX_UNIX}
RUN mkdir -p ${SDL_PREFIX_WINDOWS}
RUN mkdir -p ${ZLIB_PREFIX_UNIX}
RUN mkdir -p ${ZLIB_PREFIX_WINDOWS}
RUN mkdir -p ${LIBZIP_PREFIX_UNIX}
RUN mkdir -p ${LIBZIP_PREFIX_WINDOWS}

RUN mkdir -p ${SDL_SRC}/build
RUN mkdir -p ${SDL_IMAGE_SRC}/build
RUN mkdir -p ${ZLIB_SRC}/build
RUN mkdir -p ${LIBZIP_SRC}/build

# Unpack
RUN tar -xzf ${SDL_ARCHIVE} -C ${SDL_SRC}/ --strip-components 1 \
    && rm ${SDL_ARCHIVE}
RUN tar -xzf ${SDL_IMAGE_ARCHIVE} -C ${SDL_IMAGE_SRC}/ --strip-components 1 \
    && rm ${SDL_IMAGE_ARCHIVE}
RUN tar -xzf ${ZLIB_ARCHIVE} -C ${ZLIB_SRC}/ --strip-components 1 \
    && rm ${ZLIB_ARCHIVE}
RUN tar -xzf ${LIBZIP_ARCHIVE} -C ${LIBZIP_SRC}/ --strip-components 1 \
    && rm ${LIBZIP_ARCHIVE}

#### SDL ####

WORKDIR ${SDL_SRC}/build

RUN rm -rf ./*
# Configure, build, install for Linux
RUN cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_UNIX}

RUN cmake --build . --config Release
RUN cmake --build . --target install

# Configure, build, install for Windows
RUN rm -rf ./*
RUN cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
    -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_WINDOWS}

RUN cmake --build . --config Release
RUN cmake --build . --target install

#### SDL_Image ####

WORKDIR ${SDL_IMAGE_SRC}/build

RUN rm -rf ./*
# Install for Linux
RUN cmake .. -DCMAKE_C_COMPILER=clang \
    -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_UNIX}

RUN cmake --build . --config Release
RUN cmake --build . --target install

RUN rm -rf ./*

# Install for Windows (MinGW)
RUN cmake .. -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_WINDOWS}

RUN cmake --build . --config Release
RUN cmake --build . --target install

#### ZLIB ####

WORKDIR ${ZLIB_SRC}/build

# Linux
RUN rm -rf ./*
RUN cmake .. -DCMAKE_C_COMPILER=clang \
    -DCMAKE_INSTALL_PREFIX=${ZLIB_PREFIX_UNIX}

RUN cmake --build . --config Release
RUN cmake --build . --target install

# Windows
RUN rm -rf ./*
RUN cmake .. -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_INSTALL_PREFIX=${ZLIB_PREFIX_WINDOWS}

RUN cmake --build . --config Release
RUN cmake --build . --target install

#### LIBZIP ####

WORKDIR ${LIBZIP_SRC}/build

# Linux
RUN rm -rf ./*
RUN cmake .. -DCMAKE_C_COMPILER=clang \
    -DZLIB_LIBRARY=${ZLIB_PREFIX_UNIX}/lib/libz.a \
    -DZLIB_INCLUDE_DIR=${ZLIB_PREFIX_UNIX}/include \
    -DBUILD_SHARED_LIBS=OFF \
    -DENABLE_LZMA=OFF \
    -DCMAKE_INSTALL_PREFIX=${LIBZIP_PREFIX_UNIX}

RUN cmake --build . --config Release
RUN cmake --build . --target install

# Windows
RUN rm -rf ./*
RUN cmake .. -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DZLIB_LIBRARY=${ZLIB_PREFIX_WINDOWS}/lib/libzlibstatic.a \
    -DZLIB_INCLUDE_DIR=${ZLIB_PREFIX_WINDOWS}/include \
    -DBUILD_SHARED_LIBS=OFF \
    -DENABLE_LZMA=OFF \
    -DENABLE_WINDOWS_CRYPTO=OFF \
    -DCMAKE_INSTALL_PREFIX=${LIBZIP_PREFIX_WINDOWS}

RUN cmake --build . --config Release
RUN cmake --build . --target install

#### Cleanup ###

WORKDIR /root

RUN rm -rf ${SDL_SRC}
RUN rm -rf ${SDL_IMAGE_SRC}
RUN rm -rf ${ZLIB_SRC}
RUN rm -rf ${LIBZIP_SRC}