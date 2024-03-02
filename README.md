# Game Engine in C

Everything is in the title, this project is an attempt to write a game engine in C.

## How to get the project

You can clone it but, you will need the submodules that go with it:

```bash
git clone --recurse-submodules git@github.com:Gamabunta57/GooE.git
```

If you already have cloned the project but don't have all the submodules:

```bash
git submodule init
git submodule update
```

## How to build the project

### Some requirements

If you want to test the project as-is please notice that some images are not provided in this repository (mainly because the project doesn't aim at providing such assets and also to avoid some licensing issues). You will have to provide the files yourself of change the source code at your convenience.

The file to provide is:

- assets/img/test.png
- assets/music/test.ogg

### Building the project when all the dependencies are already built

If all the dependencies are already built, then you just have to pick one the following script and run it.

```batch
.\build-debug.bat
.\build-pre-release.bat
.\build-release.bat
```

### Building the dependencies

#### Pre-requisite

In order to build everything manually, some tools are required:

- [CMake](https://cmake.org/)
- [NASM](https://www.nasm.us/)
- [Perl](https://www.perl.org/)

Make sure these are installed on your machine and their executable directories are available in the PATH.
The following dependencies could be built the way you want of course or you can try to follow the process described.

#### SDL3

This lib has been choosen amongst other, and it has been decided to go root by building it ourselves.
The details could be found here on the [SDL official website](https://wiki.libsdl.org/SDL2/Installation).

You can build it the way you want but, this project goes with the classic CMake following the instructions of the SDL official website.

```batch
cd vendors\sdl
mkdir build
cd build
cmake.exe .. -DCMAKE_BUILD_TYPE=Release
cmake.exe --build . --config Release --parallel
cmake.exe --install . --config Release --prefix ../../build/sdl
```

This will build SDL3 for you and create the folder structure that is compatible with the batch script to build the project itself.

#### SDL_image

To load images the SDL way we need [SDL_image](https://wiki.libsdl.org/SDL2_image).
Make sure [SDL](#sdl3) is built first as it is required for SDL_image to be built.

```batch
cd vendors\sdl_image
git submodule update --init
mkdir build
cd build
cmake.exe .. -DCMAKE_BUILD_TYPE=Release -DSDL3_DIR="$pwd\..\..\build\sdl\cmake"
cmake.exe --build . --config Release --parallel
cmake.exe --install . --config Release --prefix ..\..\build\sdl_image
```

This will build SDL3_image and create the folder structure that is compatible with the batch script to build the project itself.

#### SDL_mixer

To load sound and music the SDL way we need [SDL_mixer](https://wiki.libsdl.org/SDL2_mixer).
Make sure [SDL](#sdl3) is built first as it is required for SDL_image to be built.

```batch
cd vendors\sdl_mixer
git submodule update --init
mkdir build
cd build
cmake.exe .. -DCMAKE_BUILD_TYPE=Release -DSDL3_DIR="$pwd\..\..\build\sdl\cmake"
cmake.exe --build . --config Release --parallel
cmake.exe --install . --config Release --prefix ..\..\build\sdl_mixer
```

This will build SDL3_mixer and create the folder structure that is compatible with the batch script to build the project itself.
