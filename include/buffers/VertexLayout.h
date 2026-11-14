#pragma once

#include <cstddef>
#include <vector>

// Attribute locations shared by every vertex format and by the shaders
// (layout(location = N) in shaders/*.vert).
namespace AttribLocation {
	inline constexpr unsigned int Position = 0;
	inline constexpr unsigned int Color = 1;
	inline constexpr unsigned int TexCoord = 2;
	inline constexpr unsigned int Normal = 3;
}

// Describes how float attributes are interleaved in a vertex buffer.
struct VertexAttributeDesc {
	unsigned int location;
	int components;         // number of floats
	std::size_t offsetBytes;
};

struct VertexLayout {
	std::vector<VertexAttributeDesc> attributes;
	std::size_t strideBytes = 0;
};
