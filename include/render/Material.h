#pragma once

#include <render/Shader.h>
#include <render/Texture.h>

#include <memory>

// Surface properties read by shaders/lit.frag as `uniform Material material`.
// Textures are shared: several meshes or cubes can use the same image.
// A missing map samples as black (texture 0 is bound), i.e. no diffuse or
// no specular contribution.
struct Material {
	std::shared_ptr<const Texture> diffuse;
	std::shared_ptr<const Texture> specular;
	float shininess = 32.0f;

	static constexpr unsigned int DiffuseUnit = 0;
	static constexpr unsigned int SpecularUnit = 1;

	// Binds the maps to their texture units and sets material.* uniforms.
	// The shader must be in use.
	void apply(const Shader& shader) const;
};
