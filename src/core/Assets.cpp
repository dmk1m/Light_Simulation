#include <core/Assets.h>

#include <filesystem>
#include <string>
#include <system_error>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace fs = std::filesystem;

namespace {
	fs::path executableDirectory()
	{
#ifdef _WIN32
		wchar_t buffer[MAX_PATH];
		const DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
		if (length > 0 && length < MAX_PATH)
		{
			return fs::path(buffer).parent_path();
		}
#else
		std::error_code ec;
		const fs::path self = fs::read_symlink("/proc/self/exe", ec);
		if (!ec)
		{
			return self.parent_path();
		}
#endif
		return {};
	}

	// A project root is a directory holding assets/shaders.
	bool isProjectRoot(const fs::path& dir)
	{
		std::error_code ec;
		return fs::is_directory(dir / "assets" / "shaders", ec);
	}

	fs::path findProjectRoot()
	{
		std::error_code ec;
		const fs::path cwd = fs::current_path(ec);
		if (!ec && isProjectRoot(cwd))
		{
			return cwd;
		}

		for (fs::path dir = executableDirectory(); !dir.empty(); dir = dir.parent_path())
		{
			if (isProjectRoot(dir))
			{
				return dir;
			}
			if (dir == dir.parent_path())
			{
				break;  // reached the filesystem root
			}
		}

#ifdef LIGHTSIM_SOURCE_DIR
		// Set by CMake, for build directories outside the source tree.
		if (isProjectRoot(LIGHTSIM_SOURCE_DIR))
		{
			return fs::path(LIGHTSIM_SOURCE_DIR);
		}
#endif
		return ec ? fs::path(".") : cwd;
	}
}

const fs::path& assetRoot()
{
	static const fs::path root = findProjectRoot() / "assets";
	return root;
}

fs::path assetPath(const std::string& relativePath)
{
	return assetRoot() / relativePath;
}
