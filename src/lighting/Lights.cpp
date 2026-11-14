#include <lighting/Lights.h>

#include <glm/glm.hpp>

namespace {
	void applyColors(const Shader& shader, const std::string& name, const LightColors& colors)
	{
		shader.setVec3(name + ".ambient", colors.ambient);
		shader.setVec3(name + ".diffuse", colors.diffuse);
		shader.setVec3(name + ".specular", colors.specular);
	}

	void applyAttenuation(const Shader& shader, const std::string& name, const Attenuation& attenuation)
	{
		shader.setFloat(name + ".constant", attenuation.constant);
		shader.setFloat(name + ".linear", attenuation.linear);
		shader.setFloat(name + ".quadratic", attenuation.quadratic);
	}
}

void DirectionalLight::applyTo(const Shader& shader, const std::string& name) const
{
	shader.setVec3(name + ".direction", direction);
	applyColors(shader, name, colors);
}

void PointLight::applyTo(const Shader& shader, const std::string& name) const
{
	shader.setVec3(name + ".position", position);
	applyColors(shader, name, colors);
	applyAttenuation(shader, name, attenuation);
}

void SpotLight::applyTo(const Shader& shader, const std::string& name) const
{
	shader.setVec3(name + ".position", position);
	shader.setVec3(name + ".direction", direction);
	applyColors(shader, name, colors);
	applyAttenuation(shader, name, attenuation);
	// The shader compares cosines, so pass cos(angle).
	shader.setFloat(name + ".cutOff", glm::cos(glm::radians(innerCutOffDegrees)));
	shader.setFloat(name + ".outerCutOff", glm::cos(glm::radians(outerCutOffDegrees)));
}
