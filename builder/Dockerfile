#
#   This dockerfile is to be used only to create a working image with SDL support
#

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
    libx11-dev \
    xorg-dev \
    libgl1-mesa-glx \
    libglib2.0-0 

# CMake
ADD https://github.com/Kitware/CMake/releases/download/v3.30.0/cmake-3.30.0-linux-x86_64.tar.gz /tmp
RUN tar -xzf /tmp/cmake-3.30.0-linux-x86_64.tar.gz -C /usr/local --strip-components=1 --no-same-owner && \
    rm /tmp/cmake-3.30.0-linux-x86_64.tar.gz

# Create Unix/Windows directories for SDL libs and includes
ENV SDL_PREFIX_UNIX=/opt/sdl/unix
ENV SDL_PREFIX_WINDOWS=/opt/sdl/windows

ARG SDL_SRC=/tmp/SDL_SOURCES
ARG SDL_IMAGE_SRC=/tmp/SDL_IMAGE_SOURCES

ARG SDL_ARCHIVE=/tmp/SDL.tar.gz
ARG SDL_IMAGE_ARCHIVE=/tmp/SDL_Image.tar.gz

# SDL lib
ADD https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.30.4.tar.gz ${SDL_ARCHIVE}
RUN mkdir -p ${SDL_PREFIX_UNIX} && \
    mkdir -p ${SDL_PREFIX_WINDOWS} && \
    mkdir -p ${SDL_SRC} && \
    tar -xzf ${SDL_ARCHIVE} -C ${SDL_SRC}/ --strip-components 1 && \
    rm ${SDL_ARCHIVE} && \
    mkdir -p ${SDL_SRC}/build && \
    cd ${SDL_SRC}/build && \
    rm -rf ./* && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_UNIX} && \
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
RUN mkdir -p ${SDL_IMAGE_SRC} && \
    tar -xzf ${SDL_IMAGE_ARCHIVE} -C ${SDL_IMAGE_SRC}/ --strip-components 1 && \
    rm ${SDL_IMAGE_ARCHIVE} && \
    mkdir -p ${SDL_IMAGE_SRC}/build && \
    cd ${SDL_IMAGE_SRC}/build && \
    cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_UNIX} && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cmake .. -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_INSTALL_PREFIX=${SDL_PREFIX_WINDOWS} && \
    cmake --build . --config Release && \
    cmake --build . --target install && \
    rm -rf ./* && \
    cd / && rm -rf ${SDL_IMAGE_SRC}

# Start from a Bash prompt
CMD [ "/bin/bash" ]