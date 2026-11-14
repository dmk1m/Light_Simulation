# Light_Simulation

A small OpenGL 3.3 demo that renders textured crates and a 3D model with several light types: directional, point and spot. It's a compact learning project showing how shaders, vertex buffers, textures, materials, model loading and a fly-through camera fit together.

## What you'll see
- Ten crates with diffuse and specular maps, plus a backpack model loaded with Assimp.
- Per-fragment Phong lighting from a sun (directional), four point lights (shown as small white cubes) and a flashlight (spot light) that follows the camera.
- A camera you can fly around with the mouse and keyboard.

## Controls
| Input | Action |
|---|---|
| W / A / S / D | Move forward / left / back / right |
| Mouse | Look around |
| Scroll wheel | Zoom (field of view) |
| Esc | Quit |

## Building and running

### Windows: Visual Studio
Open `Light_Simulation.slnx` (or `Light_Simulation.vcxproj`) in Visual Studio 2026, select **Debug | x64**, then build and run. GLFW and Assimp are prebuilt in `externals/libs`, and the build copies the Assimp DLL next to the executable.

### Windows: CMake
```bash
cmake -S . -B build/cmake
cmake --build build/cmake --config Debug
```
The executable is written to `build/cmake/Debug/Light_Simulation.exe`.

### Linux / macOS: CMake (untested)
Install GLFW 3.3+ and Assimp (`sudo apt install libglfw3-dev libassimp-dev`, or `brew install glfw assimp`), then:
```bash
cmake -S . -B build/cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build build/cmake
```
macOS caps OpenGL at 4.1 and has deprecated it, so expect deprecation warnings there.

### Assets
The program finds `shaders/`, `textures/` and `models/` by itself. It looks in the working directory first, then in the folders above the executable, then in the source directory recorded by CMake. You can start it from Visual Studio, by double-clicking the exe, or from any terminal.

## Project layout
| Path | Contents |
|---|---|
| `src/main.cpp` | Entry point: runs `App` and reports fatal errors |
| `src/app/` | `App`: window, camera, GPU resources and the render loop |
| `src/scene/` | `Scene`: light settings and object placement (`Scene::createDemo()`) |
| `src/lighting/` | `DirectionalLight`, `PointLight`, `SpotLight`; each writes its own shader uniforms |
| `src/render/` | `Material`: diffuse/specular maps and shininess |
| `src/models/` | Assimp model loading (`Model`) and GPU meshes (`Mesh`) |
| `src/buffers/` | RAII wrappers for VAOs, vertex and index buffers, plus `VertexLayout` |
| `src/shader/`, `src/texture/` | Shader program and texture wrappers |
| `src/shapes/` | `Box`: cube geometry generation |
| `src/camera/`, `src/window/` | Fly-through camera and the GLFW window and input |
| `src/core/` | Colors, asset path lookup, OpenGL debug output |
| `shaders/` | GLSL: `cube.*` for lit surfaces, `light.*` for the light markers |

Every class that wraps an OpenGL object owns it: it frees the object when destroyed and can be moved but not copied.

## Troubleshooting
- **Console messages:** a failed shader compile or link, or a missing texture, model or shader file, is printed as `Fatal error: ...` and the program exits.
- **`[GL ...]` lines:** debug builds request an OpenGL debug context and print driver errors and warnings to the console. NVIDIA drivers report one harmless "shader recompiled" performance note per shader at startup.
- **Release builds:** `externals/libs` only contains the debug build of Assimp, which is also used for Release. Add a release build of Assimp to use a matching runtime.

## Credits and license
- Backpack model: see `models/backpack/source_attribution.txt`.
- Code license: see `LICENSE`.
