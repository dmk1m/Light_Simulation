#include <scene/Scene.h>

Scene Scene::createDemo()
{
	Scene scene;

	scene.sun.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	scene.sun.colors = { glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f) };

	const glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	for (const glm::vec3& position : pointLightPositions)
	{
		PointLight light;
		light.position = position;
		light.colors = { glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f) };
		scene.pointLights.push_back(light);
	}

	scene.flashlight.colors = { glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f) };
	scene.flashlight.innerCutOffDegrees = 12.5f;
	scene.flashlight.outerCutOffDegrees = 15.0f;

	scene.cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Off to the left so the crate at the origin doesn't hide it from the start position.
	scene.backpackPosition = glm::vec3(-3.0f, 0.0f, -5.0f);
	scene.backpackScale = 0.5f;

	return scene;
}
