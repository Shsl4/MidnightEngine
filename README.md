# MidnightEngine

 [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)
 ![RepoSize](https://img.shields.io/github/repo-size/Shsl4/MidnightEngine)
 [![Codacy Badge](https://app.codacy.com/project/badge/Grade/5086606e49914fe0acbcdbe71dce5e9d)](https://www.codacy.com/gh/Shsl4/MidnightEngine/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Shsl4/MidnightEngine&amp;utm_campaign=Badge_Grade)
 
## A cross-platform 3D rendering engine written in C++

## Requirements

This project requires [CMake](https://cmake.org/download/) version 3.8 or above. The project only targets x64 and arm64.

## Building

Clone the project and move to the directory:
```sh
git clone https://github.com/Shsl4/MidnightEngine
cd MidnightEngine
```
### Windows
On Windows, generate the Visual Studio 2022 project:
```bat
.\generateVS2022.bat
```
### macOS
On macOS, generate the Xcode project:
```sh
chmod +x generateXCode.sh
./generateXcode.sh
```
### Linux
On Linux, either open the cloned directory as a project using a CMake compatible IDE such as [CLion](https://www.jetbrains.com/clion/), or generate a makefile project using:
```sh
chmod +x generateMakefile.sh
./generateMakefile.sh
```

> ⚠️ The project does not contain the Linux SDL2 binaries. You have to install them using your package manager.

The Visual Studio, Xcode and Makefile projects will be generated in the hidden .build directory.
Alternatively, you can generate the project yourself using cmake.

## Documentation

You can generate documentation for this project using doxygen.
Simply move to the root directory and type:
```sh
doxygen
```
You will find the documentation in the Docs directory. Otherwise, the docs are available online [here](https://shsl4.github.io/MidnightEngine/)

## Third-Party Libraries

-   [tinystl](https://github.com/mendsley/tinystl) - Tiny (as in minimal) implementation of some core STL functionality 
-   [SDL2](https://github.com/libsdl-org/SDL) - Simple Directmedia Layer 
-   [fmt](https://github.com/fmtlib/fmt) - A modern formatting library 
-   [bgfx](https://github.com/bkaradzic/bgfx) - Cross-platform, graphics API agnostic, "Bring Your Own Engine/Framework" style rendering library. 
-   [imgui](https://github.com/ocornut/imgui) - Bloat-free Graphical User interface for C++ with minimal dependencies 
-   [iconfontheaders](https://github.com/juliettef/IconFontCppHeaders) - C, C++ headers and C# classes for icon fonts: Font Awesome, Fork Awesome, Material Design, Kenney game icons and Fontaudio 
-   [doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css) - Custom CSS theme for doxygen html-documentation with lots of customization parameters. 
-   [assimp](https://github.com/assimp/assimp) - The official Open-Asset-Importer-Library Repository. Loads 40+ 3D-file-formats into one unified and clean data structure. 

For now, binaries provided for third-party libraries are built for Windows x64, Linux x64 and macOS arm64 only.
