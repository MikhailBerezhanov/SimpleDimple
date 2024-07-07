## Prerequisites
GLIBC >= 2.28 for Linux

## Docker builder
Project uses docker image for multi-platfrom application building:
https://hub.docker.com/r/drone29/sdl2-container

```
docker pull drone29/sdl2-container
```

### Building with terminal
Prepare build directory
```
mkdir -p build
cd build
```

Build steps for `Linux` target:
```
cmake .. --preset "SDL for Linux CLang"
cmake --build Linux/
cmake --build Linux/ --target install
```

Build steps for `Windows` target:
```
cmake .. --preset "SDL for Windows MinGW"
cmake --build Windows/
cmake --build Windows/ --target install
```

After installation, the files should be arranged as follows:

Linux:
```
bin/
    your_app_executable
    SDL2.so
    ...all_other_sdl_libs.so
```
Windows:
```
bin/
    your_app.exe
    SDL2.dll
```

### Bulding with VSCode

Using VSCode 'Dev Containers' extension:
```
Ctrl+Shft+P
>Dev Containers: Open Folder in Container
Use local workspace
Use DockerFile
```
After that VSCode should run the container in accordance with Dockerfile contents

The project folder is mounted automatically into the container, 
so any changes to sources are preserved and may be later accessed outside of the container

Also, there's a ```run_in_docker.sh``` script which is meant to mimic 'Dev Containers' behaviour (Linux only)

It's also possible to use ```CMakePresets.json``` to configure project for Linux or Windows target with VSCode:

```>Cmake: Select Configure preset```

## Xhost
Allow Docker to use X11 server for video:
```
xhost +local:docker
```
Remove Docker from X11 list:
```
xhost -local:docker
```

#### Linkage:

See PlatformDependencies.cmake
