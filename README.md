## SDL2
Build SDL2:

Build for Linux with clang (default):
```
./build_sdl.sh
```
Build for Windows with MinGW (cross-compile)
```
./build_sdl.sh windows
```
The following folders are created after that:
```
bin
include
lib
share
```
It's also possible to build SDL on Windows with MSVC+Clang (see build_sdl.ps1)

### Configure
```
mkdir build
cd build
cmake ..
```
### Build
```
cd build
cmake --build .
```
### Install
```
cmake --build . --target install
```

## Docker
Tested only with VSCode 'Dev Containers' extension:
```
Ctrl+Shft+P
>Dev Containers: Open Folder in Container
Use local workspace
Use DockerFile
```
After that VSCode should run the container in accordance with Dockerfile contents

The project folder is mapped automatically into the container, 
so any changes to sources are preserved and may be later accessed outside of the container

## Xhost
Allow Docker to use X11 server for video:
```
xhost +local:docker
```
Remove Docker from X11 list:
```
xhost -local:docker
```