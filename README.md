# Game Creation Tool-kit (GCTk)
GCTk is a 2D game engine with Lua bindings.<br/>
Dependencies are included as submodules, except GLEW.

## Submodules:
- [glfw](https://github.com/glfw/glfw)
- [cglm](https://github.com/recp/cglm)
- [lua with cmake](https://github.com/walterschell/Lua)
- [SOIL2](https://github.com/SpartanJ/SOIL2.git)
- [GctkImGui](https://github.com/DatSpygineer/GctkImGui) (cimgui custom fork)

GLEW is not included as a submodule due to build issues on Windows.<br/>
On Windows GLEW needs to be downloaded manually.<br/>
You can download the binaries from their website: https://glew.sourceforge.net/ <br/>
Copy "**glew-2.1.0**" folder into the "**libs**" folder.

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
- Gctk Project manager -> Tool for generating/building project for your game.
- Gctk Texture tool -> Tool for converting texture formats.
- Gctk Preprocessor -> Tool for generating metadata.
- Gctk Editor -> Fully featured editor for the game engine.