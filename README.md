# Light_Simulation

A small, self-contained OpenGL demo that renders textured cubes with multiple light types (directional, point, and spot). It’s intended as a compact learning example showing how shaders, vertex buffers, textures and a simple camera fit together.

What you’ll see
- A windowed demo that draws multiple textured cubes and small cubes representing point lights.
- Per-fragment lighting (ambient/diffuse/specular) using GLSL shaders.
- Camera you can move/look around (mouse + keyboard input handled by the window layer).

Quick facts
- Languages: C and C++ with GLSL shader sources.
- Entry point: `src/main.cpp`
- Shaders: `shaders/cube.vert`, `shaders/cube.frag`, `shaders/light.vert`, `shaders/light.frag`
- Textures: `textures/container2.png`, `textures/container2_specular.png`
- Includes GL loader (src/glad.c), GLM for math, and stb_image for textures.

How to run
- Windows (recommended): Open `Light_Simulation.slnx` or `Light_Simulation.vcxproj` in Visual Studio, build and run. Make sure the working directory contains the `shaders/` and `textures/` folders so the program can load assets.
- Cross-platform: Add a small build system (CMake) and link OpenGL + a window library (e.g., GLFW). The code uses relative paths for shaders/textures.

Where to look first
- `src/main.cpp` — app startup, shader & texture setup, render loop.
- `src/shader/OpenGLShader.*` — shader loading and uniform helpers.
- `src/shapes/Box.*` — how cube vertices are generated.
- `shaders/` — GLSL code that implements the lighting.

Notes
- If shaders fail to compile, check console output; shader compilation messages are printed.
- Texture loading throws on failure — ensure texture files exist and the executable’s working directory is correct.

License
- See the `LICENSE` file in the repository root.

If you want, I can create a minimal CMakeLists.txt so this builds on Linux/macOS, or shorten the build instructions into a one-line summary.
