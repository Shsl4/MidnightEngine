# MidnightEngine
### A cross-platform 3D rendering engine written in C++
## Requirements
This project requires cmake version 3.8 or above. The project only targets x64 and arm64.
## Building
Clone the project and move to the directory:
```sh
git clone https://github.com/Shsl4/MidnightEngine
cd MidnightEngine
```
On Windows, generate the Visual Studio 2022 project:
```bat
.\generateVS2022.bat
```
On macOS, generate the Xcode project:
```sh
chmod +x generateXCode.sh
./generateXcode.sh
```
The Visual Studio and Xcode projects will be generated in the Build directory.
Alternatively, you can generate the project yourself using cmake.
## Documentation
You can generate documentation for this project using doxygen.
Simply move to the root directory and type:
```sh
doxygen
```
You will find the documentation in the Docs directory. 
## Third-Party Libraries
- [tinystl] - Tiny (as in minimal) implementation of some core STL functionality 
- [SDL2] - Simple Directmedia Layer 
- [fmt] - A modern formatting library 
- [bgfx] - Cross-platform, graphics API agnostic, "Bring Your Own Engine/Framework" style rendering library. 
- [imgui] - Bloat-free Graphical User interface for C++ with minimal dependencies 
- [iconfontheaders] - C, C++ headers and C# classes for icon fonts: Font Awesome, Fork Awesome, Material Design, Kenney game icons and Fontaudio 

For now, binaries provided for third-party libraries are built for Windows x64 and macOS arm64 only.

[tinystl]: <https://github.com/mendsley/tinystl>
[SDL2]: <https://github.com/libsdl-org/SDL>
[fmt]: <https://github.com/fmtlib/fmt>
[bgfx]: <https://github.com/bkaradzic/bgfx>
[imgui]: <https://github.com/ocornut/imgui>
[iconfontheaders]: <https://github.com/juliettef/IconFontCppHeaders>
