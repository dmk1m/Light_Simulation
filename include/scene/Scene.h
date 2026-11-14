#pragma once

#include <core/Color.h>
#include <lighting/Lights.h>

#include <glm/glm.hpp>

#include <vector>

// What is in the world: lights and object placements. Pure data; App owns
// the GPU resources used to draw it.
struct Scene {
	Color backgroundColor = Color::Gray;

	DirectionalLight sun;
	std::vector<PointLight> pointLights;   // at most MaxPointLights
	SpotLight flashlight;                  // App moves it with the camera each frame

	std::vector<glm::vec3> cubePositions;
	float cubeShininess = 64.0f;

	glm::vec3 backpackPosition{ 0.0f };
	float backpackScale = 1.0f;

	float lightCubeScale = 0.2f;

	// The demo scene: ten crates, four point lights, a sun, a flashlight and
	// the backpack model.
	static Scene createDemo();
};
