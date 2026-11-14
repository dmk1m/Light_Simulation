#include <app/App.h>

#include <core/Assets.h>
#include <window/GLFWindow.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <memory>
#include <string>

namespace {
	constexpr int WindowWidth = 1920;
	constexpr int WindowHeight = 1080;
	constexpr float NearPlane = 0.1f;
	constexpr float FarPlane = 100.0f;

	std::shared_ptr<const Texture> loadTexture(const std::string& relativePath)
	{
		return std::make_shared<const Texture>(assetPath(relativePath).string());
	}
}

App::App()
	: m_camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	m_window(std::make_unique<GLFWindow>(WindowWidth, WindowHeight, "Light Simulation")),
	m_scene(Scene::createDemo()),
	m_litShader(assetPath("shaders/lit.vert").string(), assetPath("shaders/lit.frag").string()),
	m_unlitShader(assetPath("shaders/unlit.vert").string(), assetPath("shaders/unlit.frag").string()),
	m_backpack(assetPath("models/backpack/backpack.obj"))
{
	m_window->setCamera(m_camera);
	m_window->lockCursor();

	m_cube.includeTextureCoordinates();
	m_cube.includeNormal();
	uploadBox(m_cube, m_cubeVbo, m_cubeVao);
	uploadBox(m_lightCube, m_lightCubeVbo, m_lightCubeVao);

	m_crateMaterial.diffuse = loadTexture("textures/container2.png");
	m_crateMaterial.specular = loadTexture("textures/container2_specular.png");
	m_crateMaterial.shininess = m_scene.cubeShininess;

	applyStaticLighting();
}

void App::uploadBox(const Box& box, VertexBuffer& vbo, VertexArray& vao)
{
	vao.bind();
	vbo.setData(box.getVertices());
	vao.setLayout(box.getLayout());
	vao.unbind();
	vbo.unbind();
}

void App::applyStaticLighting()
{
	// Uniforms are per-program state, so lights that never move are set once.
	m_litShader.use();
	m_scene.sun.applyTo(m_litShader);

	const int count = std::min(static_cast<int>(m_scene.pointLights.size()), MaxPointLights);
	m_litShader.setInt("pointLightCount", count);
	for (int i = 0; i < count; i++)
	{
		m_scene.pointLights[i].applyTo(m_litShader, "pointLights[" + std::to_string(i) + "]");
	}
}

void App::run()
{
	while (!m_window->shouldClose())
	{
		m_window->processInput();
		renderFrame();
		m_window->update();
	}
}

glm::mat4 App::projectionMatrix() const
{
	// Height is 0 while the window is minimized.
	const int width = m_window->getWidth();
	const int height = m_window->getHeight();
	const float aspect = height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
	return glm::perspective(glm::radians(m_camera.getZoom()), aspect, NearPlane, FarPlane);
}

void App::renderFrame()
{
	m_window->setBackgroundColor(m_scene.backgroundColor);
	m_window->clearDepthBuffer();

	const glm::mat4 view = m_camera.getViewMatrix();
	const glm::mat4 projection = projectionMatrix();

	// Lit objects
	m_litShader.use();
	m_litShader.setMat4("view", view);
	m_litShader.setMat4("projection", projection);
	m_litShader.setVec3("viewPos", m_camera.getPosition());

	m_scene.flashlight.position = m_camera.getPosition();
	m_scene.flashlight.direction = m_camera.getFront();
	m_scene.flashlight.applyTo(m_litShader);

	m_crateMaterial.apply(m_litShader);
	m_cubeVao.bind();
	for (const glm::vec3& position : m_scene.cubePositions)
	{
		m_litShader.setMat4("model", glm::translate(glm::mat4(1.0f), position));
		glDrawArrays(GL_TRIANGLES, 0, m_cube.getVertexCount());
	}

	glm::mat4 backpackModel = glm::translate(glm::mat4(1.0f), m_scene.backpackPosition);
	backpackModel = glm::scale(backpackModel, glm::vec3(m_scene.backpackScale));
	m_litShader.setMat4("model", backpackModel);
	m_backpack.draw(m_litShader);

	// Point light markers
	m_unlitShader.use();
	m_unlitShader.setMat4("view", view);
	m_unlitShader.setMat4("projection", projection);
	m_unlitShader.setVec3("lightColor", m_lightCube.getColorVec3());

	m_lightCubeVao.bind();
	for (const PointLight& light : m_scene.pointLights)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), light.position);
		model = glm::scale(model, glm::vec3(m_scene.lightCubeScale));
		m_unlitShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, m_lightCube.getVertexCount());
	}
	m_lightCubeVao.unbind();
}
