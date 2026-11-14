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
The program finds its `assets/` folder by itself. It looks in the working directory first, then in the folders above the executable, then in the source directory recorded by CMake. You can start it from Visual Studio, by double-clicking the exe, or from any terminal.

## Project layout
Headers live in `include/` and mirror the folders in `src/`.

| Path | Contents |
|---|---|
| `src/main.cpp` | Entry point: runs `App` and reports fatal errors |
| `src/app/` | `App`: window, camera, GPU resources and the render loop |
| `src/scene/` | `Scene`: light settings and object placement (`Scene::createDemo()`) |
| `src/lighting/` | `DirectionalLight`, `PointLight`, `SpotLight`; each writes its own shader uniforms |
| `src/render/` | OpenGL wrappers: `Shader`, `Texture`, `VertexArray`, `VertexBuffer`, `IndexBuffer`, `VertexLayout`, `Material`, debug output |
| `src/geometry/` | `Box` cube generation, Assimp model loading (`Model`) and GPU meshes (`Mesh`) |
| `src/camera/` | `Camera` interface and the `FlyCamera` implementation |
| `src/window/` | `Window` interface and the GLFW implementation, including input |
| `src/core/` | `Color` and asset path lookup |
| `assets/shaders/` | GLSL: `lit.*` for textured, lit surfaces; `unlit.*` for the light markers |
| `assets/textures/`, `assets/models/` | Images and the backpack model |
| `externals/` | Third-party headers (`includes/`), sources compiled into the program (`src/`: glad, stb_image) and prebuilt libraries (`libs/`: GLFW, Assimp) |

Every class that wraps an OpenGL object owns it: it frees the object when destroyed and can be moved but not copied.

## Troubleshooting
- **Console messages:** a failed shader compile or link, or a missing texture, model or shader file, is printed as `Fatal error: ...` and the program exits.
- **`[GL ...]` lines:** debug builds request an OpenGL debug context and print driver errors and warnings to the console. NVIDIA drivers report one harmless "shader recompiled" performance note per shader at startup.
- **Release builds:** `externals/libs` only contains the debug build of Assimp, which is also used for Release. Add a release build of Assimp to use a matching runtime.

## Credits and license
- Backpack model: see `assets/models/backpack/source_attribution.txt`.
- Code license: see `LICENSE`.
