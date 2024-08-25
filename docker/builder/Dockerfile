FROM ubuntu:jammy

#### Dependencies ####
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    clang \
    build-essential \
    xutils-dev \
    libreadline6-dev \
    libncurses5-dev \
    mingw-w64 \
    gdb \
    git \
    procps \
    libx11-dev \
    xorg-dev \
    libgl1-mesa-glx \
    libglib2.0-0 \
    curl \
    && rm -rf /var/lib/apt/lists/*

# CMake
ADD https://github.com/Kitware/CMake/releases/download/v3.30.0/cmake-3.30.0-linux-x86_64.tar.gz /tmp
RUN tar -xzf /tmp/cmake-3.30.0-linux-x86_64.tar.gz -C /usr/local --strip-components=1 --no-same-owner && \
    rm /tmp/cmake-3.30.0-linux-x86_64.tar.gz

# Prepare environment for Linux/Windows builds
ENV SDL_PREFIX_LINUX=/opt/sdl/linux
ENV SDL_PREFIX_WINDOWS=/opt/sdl/windows
ENV ZLIB_PREFIX_LINUX=/opt/zlib/linux
ENV ZLIB_PREFIX_WINDOWS=/opt/zlib/windows
ENV LIBZIP_PREFIX_LINUX=/opt/libzip/linux
ENV LIBZIP_PREFIX_WINDOWS=/opt/libzip/windows

ARG SDL_SRC=/tmp/sdl_sources
ARG SDL_IMAGE_SRC=/tmp/sdl_image_sources
ARG SDL_TTF_SRC=/tmp/sdl_ttf_sources
ARG ZLIB_SRC=/tmp/zlib_sources
ARG LIBZIP_SRC=/tmp/libzip_sources

ARG SDL_ARCHIVE=/tmp/SDL.tar.gz
ARG SDL_IMAGE_ARCHIVE=/tmp/SDL_Image.tar.gz
ARG SDL_TTF_ARCHIVE=/tmp/SDL_TTF.tar.gz
ARG ZLIB_ARCHIVE=/tmp/ZLIB.tar.gz
ARG LIBZIP_ARCHIVE=/tmp/LIBZIP.tar.gz
# SDL_image deps
ARG LIBJPEG_ARCHIVE=/tmp/LIBJPEG.tar.gz
ARG LIBTIFF_ARCHIVE=/tmp/LIBTIFF.tar.gz
ARG SDL_ZLIB_ARCHIVE=/tmp/SDL_ZLIB.tar.gz
ARG LIBPNG_ARCHIVE=/tmp/LIBPNG.tar.gz
# SDL_ttf deps
ARG FREETYPE_ARCHIVE=/tmp/FREETYPE.tar.gz

# SDL lib
ADD https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.30.4.tar.gz ${SDL_ARCHIVE}
RUN mkdir -p ${SDL_PREFIX_LINUX} && \
    mkdir -p ${SDL_PREFIX_WINDOWS} && \
    mkdir -p ${SDL_SRC} && \
    tar -xzf ${SDL_ARCHIVE} -C ${SDL_SRC}/ --strip-components 1 && \
    rm ${SDL_ARCHIVE} && \
    mkdir -p ${SDL_SRC}/build && \
    cd ${SDL_SRC}/build && \
    rm -rf ./* && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_LINUX} && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_WINDOWS} && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cd / && rm -rf ${SDL_SRC}

# SDL_Image lib
ADD https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.8.2.tar.gz ${SDL_IMAGE_ARCHIVE}
ADD https://github.com/libsdl-org/zlib/archive/refs/tags/v1.3.1.tar.gz ${SDL_ZLIB_ARCHIVE}
ADD https://github.com/libsdl-org/libtiff/archive/refs/tags/v4.6.0.tar.gz ${LIBTIFF_ARCHIVE}
ADD https://github.com/libsdl-org/libpng/archive/refs/tags/v1.6.43.tar.gz ${LIBPNG_ARCHIVE}
# For some reason ADD doesn't work
RUN curl -k -o ${LIBJPEG_ARCHIVE} https://www.ijg.org/files/jpegsrc.v9f.tar.gz

RUN mkdir -p ${SDL_PREFIX_LINUX} && \
    mkdir -p ${SDL_PREFIX_WINDOWS} && \
    mkdir -p ${SDL_IMAGE_SRC} && \
    tar -xzf ${SDL_IMAGE_ARCHIVE} -C ${SDL_IMAGE_SRC}/ --strip-components 1 && \
    tar -xzf ${LIBJPEG_ARCHIVE} -C ${SDL_IMAGE_SRC}/external/jpeg --strip-components 1 && \
    tar -xzf ${SDL_ZLIB_ARCHIVE} -C ${SDL_IMAGE_SRC}/external/zlib --strip-components 1 && \
    tar -xzf ${LIBTIFF_ARCHIVE} -C ${SDL_IMAGE_SRC}/external/libtiff --strip-components 1 && \
    tar -xzf ${LIBPNG_ARCHIVE} -C ${SDL_IMAGE_SRC}/external/libpng --strip-components 1 && \
    rm ${SDL_IMAGE_ARCHIVE} && \
    rm ${LIBJPEG_ARCHIVE} && \
    rm ${SDL_ZLIB_ARCHIVE} && \
    rm ${LIBTIFF_ARCHIVE} && \
    rm ${LIBPNG_ARCHIVE} && \
    mkdir -p ${SDL_IMAGE_SRC}/build && \
    cd ${SDL_IMAGE_SRC}/build && \
    cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_LINUX} -DSDL2IMAGE_VENDORED=true && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cmake .. -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_WINDOWS} -DSDL2IMAGE_VENDORED=true && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cd / && rm -rf ${SDL_IMAGE_SRC}

# SDL_ttf lib
ADD https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-2.22.0.tar.gz ${SDL_TTF_ARCHIVE}
ADD https://download.savannah.gnu.org/releases/freetype/freetype-2.13.2.tar.gz ${FREETYPE_ARCHIVE}
RUN mkdir -p ${SDL_PREFIX_LINUX} && \
    mkdir -p ${SDL_PREFIX_WINDOWS} && \
    mkdir -p ${SDL_TTF_SRC} && \
    tar -xzf ${SDL_TTF_ARCHIVE} -C ${SDL_TTF_SRC}/ --strip-components 1 && \
    tar -xzf ${FREETYPE_ARCHIVE} -C ${SDL_TTF_SRC}/external/freetype --strip-components 1 && \
    rm ${SDL_TTF_ARCHIVE} && \
    rm ${FREETYPE_ARCHIVE} && \
    mkdir -p ${SDL_TTF_SRC}/build && \
    cd ${SDL_TTF_SRC}/build && \
    cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_LINUX} -DSDL2TTF_VENDORED=true && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cmake .. -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_WINDOWS} -DSDL2TTF_VENDORED=true && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cd / && rm -rf ${SDL_TTF_SRC}

# zlib
ADD https://github.com/madler/zlib/archive/refs/tags/v1.3.1.tar.gz ${ZLIB_ARCHIVE}
RUN mkdir -p ${ZLIB_PREFIX_LINUX} && \
    mkdir -p ${ZLIB_PREFIX_WINDOWS} && \
    mkdir -p ${ZLIB_SRC}/build && \
    tar -xzf ${ZLIB_ARCHIVE} -C ${ZLIB_SRC}/ --strip-components 1 && \
    rm ${ZLIB_ARCHIVE} && \
    cd ${ZLIB_SRC}/build && \
    cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=${ZLIB_PREFIX_LINUX} && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cmake .. -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_INSTALL_PREFIX=${ZLIB_PREFIX_WINDOWS} && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cd / && rm -rf ${ZLIB_SRC}

# libzip
ADD https://github.com/nih-at/libzip/archive/refs/tags/v1.10.1.tar.gz ${LIBZIP_ARCHIVE}
RUN mkdir -p ${LIBZIP_PREFIX_LINUX} && \
    mkdir -p ${LIBZIP_PREFIX_WINDOWS} && \
    mkdir -p ${LIBZIP_SRC}/build && \
    tar -xzf ${LIBZIP_ARCHIVE} -C ${LIBZIP_SRC}/ --strip-components 1 && \
    rm ${LIBZIP_ARCHIVE} && \
    cd ${LIBZIP_SRC}/build && \
    cmake .. -DCMAKE_C_COMPILER=clang -DZLIB_LIBRARY=${ZLIB_PREFIX_LINUX}/lib/libz.a -DZLIB_INCLUDE_DIR=${ZLIB_PREFIX_LINUX}/include -DBUILD_SHARED_LIBS=OFF -DENABLE_LZMA=OFF -DCMAKE_INSTALL_PREFIX=${LIBZIP_PREFIX_LINUX} && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cmake .. -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_SYSTEM_NAME=Windows -DZLIB_LIBRARY=${ZLIB_PREFIX_WINDOWS}/lib/libzlibstatic.a -DZLIB_INCLUDE_DIR=${ZLIB_PREFIX_WINDOWS}/include -DBUILD_SHARED_LIBS=OFF -DENABLE_LZMA=OFF -DENABLE_WINDOWS_CRYPTO=OFF -DCMAKE_INSTALL_PREFIX=${LIBZIP_PREFIX_WINDOWS} && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cd / && rm -rf ${LIBZIP_SRC}

# Start from a Bash prompt
CMD [ "/bin/bash" ]