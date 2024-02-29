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

### When all the dependencies are already built

If all the dependencies are already built, then you just have to pick one the following script and run it.

```batch
.\build-debug.bat
.\build-pre-release.bat
.\build-release.bat
```

### Building the dependencies

#### SDL3

This lib has been choosen amongst other, and it has been decided to go root by building it ourselves.

The details could be found here on the [SDL official website](https://wiki.libsdl.org/SDL2/Installation).

You can build it the way you want but, this project goes with the classic CMake following the instructions of the SDL official website.

Assuming you have CMake already installed and available in your PATH (otherwise check their [website](https://cmake.org/)).

```batch
cd vendors\sdl
mkdir build
cd build
cmake.exe .. -DCMAKE_BUILD_TYPE=Release
cmake.exe --build . --config Release --parallel
cmake.exe --install . --config Release --prefix ../../../vendors\build\sdl
```

This will build SDL3 for you and create the folder structure that is compatible with the batch script to build the project itself.

Once done you can follow the instructions [here](#when-all-the-dependencies-are-already-built).
