#pragma once

#include <shader/OpenGLShader.h>

#include <glm/glm.hpp>

#include <string>

// Light types matching the structs in shaders/cube.frag. Each applyTo()
// writes the light's uniforms; the shader must be in use.

// Must match MAX_POINT_LIGHTS in shaders/cube.frag.
inline constexpr int MaxPointLights = 8;

struct Attenuation {
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

struct LightColors {
	glm::vec3 ambient{ 0.0f };
	glm::vec3 diffuse{ 1.0f };
	glm::vec3 specular{ 1.0f };
};

struct DirectionalLight {
	glm::vec3 direction{ 0.0f, -1.0f, 0.0f };
	LightColors colors;

	void applyTo(const OpenGLShader& shader, const std::string& name = "dirLight") const;
};

struct PointLight {
	glm::vec3 position{ 0.0f };
	LightColors colors;
	Attenuation attenuation;

	// name is the full array element, e.g. "pointLights[2]".
	void applyTo(const OpenGLShader& shader, const std::string& name) const;
};

struct SpotLight {
	glm::vec3 position{ 0.0f };
	glm::vec3 direction{ 0.0f, 0.0f, -1.0f };
	LightColors colors;
	Attenuation attenuation;
	float innerCutOffDegrees = 12.5f;   // full intensity inside this cone
	float outerCutOffDegrees = 15.0f;   // fades to zero at this cone

	void applyTo(const OpenGLShader& shader, const std::string& name = "spotLight") const;
};
