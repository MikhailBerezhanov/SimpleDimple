## Usage
Build SDL2:
```
./build_sdl
```
The following folders are created after that:
```
bin
include
lib
share
```
Use clang to build the executable, though GCC should work too

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