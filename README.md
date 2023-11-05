# Game Creation Tool-kit (GCTk)
GCTk is a 2D game engine with Lua bindings.<br/>
All dependencies are included as submodules.

## Submodules:
- [glfw](https://github.com/glfw/glfw)
- [glew with cmake](https://github.com/Perlmint/glew-cmake.git)
- [cglm](https://github.com/recp/cglm)
- [lua with cmake](https://github.com/walterschell/Lua)
- [SOIL2](https://github.com/SpartanJ/SOIL2.git)
- [GctkImGui](https://github.com/DatSpygineer/GctkImGui) (cimgui custom fork)

## Building
### Core library and test application:
Clone the repository recursively:
```shell
git clone https://github.com/DatSpygineer/Gctk --recursive
```
Build the project using CMake in terminal:
```shell
mkdir buid
cd build
cmake ..
make all
```
On Windows, you can open the folder with Visual studio and build project.

### Build tools:
Build tools uses dotnet 7.

Currently, no build tools are implemented!
What to expect:
- Gctk Project manager -> Tool for generating project for your game.
- Gctk Texture tool -> Tool for converting texture formats.
- Gctk Preprocessor -> Tool for generating metadata.
- Gctk Editor -> Fully featured editor for the game engine.