#include <render/Material.h>

#include <glad/glad.h>

namespace {
	void bindOrClear(const std::shared_ptr<const Texture>& texture, unsigned int unit)
	{
		if (texture)
		{
			texture->bindToUnit(unit);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void Material::apply(const Shader& shader) const
{
	shader.setInt("material.diffuse", static_cast<int>(DiffuseUnit));
	shader.setInt("material.specular", static_cast<int>(SpecularUnit));
	shader.setFloat("material.shininess", shininess);

	bindOrClear(diffuse, DiffuseUnit);
	bindOrClear(specular, SpecularUnit);
	glActiveTexture(GL_TEXTURE0);
}
