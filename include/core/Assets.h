#pragma once

#include <filesystem>
#include <string>

// Resolves a path relative to the asset root: the nearest directory at or
// above the executable's folder that contains a `shaders` folder (the
// project root in a normal build), then the source directory recorded by
// CMake (LIGHTSIM_SOURCE_DIR), then the working directory.
// This lets the program run from Visual Studio or by double-clicking the exe
// in build/bin/... without copying assets.
std::filesystem::path assetPath(const std::string& relativePath);

// The directory assetPath() resolves against.
const std::filesystem::path& assetRoot();
