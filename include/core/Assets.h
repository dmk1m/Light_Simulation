#pragma once

#include <filesystem>
#include <string>

// Resolves a path inside the project's `assets` folder, e.g.
// assetPath("shaders/lit.vert"). The project root is the first of: the
// working directory, the nearest directory at or above the executable's
// folder, or the source directory recorded by CMake (LIGHTSIM_SOURCE_DIR)
// that contains assets/shaders. This lets the program run from Visual Studio
// or by double-clicking the exe in build/bin/... without copying assets.
std::filesystem::path assetPath(const std::string& relativePath);

// The `assets` directory assetPath() resolves against.
const std::filesystem::path& assetRoot();
