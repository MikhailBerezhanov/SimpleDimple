## Prerequisites
GLIBC >= 2.28 for Linux

## Configure, Build, Install in Docker

Example for Linux:
```
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_INSTALL_PREFIX=${PROJECT_ROOT_LOCATION_HERE}
cmake --build .
cmake --build . --target install
```

For Windows:
```
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
        -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_INSTALL_PREFIX=${PROJECT_ROOT_LOCATION_HERE}
cmake --build .
cmake --build . --target install
```

It's possible to use ```CMakePresets.json``` to configure project for Linux or Windows with VSCode:

```>Cmake: Select Configure preset```

(though it's rather buggy and you need to manually clear build/ folder each time you switch the preset)

After installation, the files should be arranged as follows:

Linux:
```
bin/
    your_app_executable
lib/
    SDL2.so
    ...all_other_sdl_libs.so
```
Windows:
```
bin/
    your_app.exe
    SDL2.dll
```

## Docker
Use with this docker image:
https://hub.docker.com/r/drone29/sdl2-container

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

Also, there's a ```run_in_docker.sh``` script which is meant to mimic 'Dev Containers' behaviour (Linux only)

## Xhost
Allow Docker to use X11 server for video:
```
xhost +local:docker
```
Remove Docker from X11 list:
```
xhost -local:docker
```

## Building on Host
It's possible to build the project directly on the host machine

First, build the SDL library using

```build_sdl.sh``` for Linux (Clang)

```build_sdl.ps1``` for Windows (Clang)

After that, these directories are created:
```
bin/ - contains sdl2-config (and SDL2.dll runtime lib for Windows)
lib/ - contains .so/.a libs for Linux or "import" libs for Windows
```

#### Linkage:

```libSDL2.so``` for Linux (Clang)

```libSDL2main.a``` ```libSDL2.a``` for Windows (MinGW)

```libSDL2d.lib``` ```libSDL2maind.lib``` for Windows (Clang)

Runtime lib for Windows ```SDL2.dll``` should be in the same folder as the executable

Runtime libs for Linux ```.so``` should be placed in ```$ORIGIN/..lib/``` folder, 
where ```$ORIGIN``` is the executable location 

You can override ```SDL_PREFIX``` with cmake cli parameters to specify SDL installation location:

```
cd build
cmake .. -DSDL_PREFIX=<PROJECT_ROOT_LOCATION_HERE>
cmake --build .
cmake --build . --target install
```

After installation the folders' structure should be the same as described in the first part
