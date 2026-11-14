#pragma once

#include <render/VertexArray.h>
#include <render/VertexBuffer.h>
#include <camera/FlyCamera.h>
#include <geometry/Model.h>
#include <render/Material.h>
#include <scene/Scene.h>
#include <render/Shader.h>
#include <geometry/Box.h>
#include <window/Window.h>

#include <glm/glm.hpp>

#include <memory>

// Owns the window, camera, GPU resources and scene, and runs the render loop.
// The constructor throws std::runtime_error if any resource fails to load.
class App {
public:
	App();
	void run();

private:
	// Declaration order matters: members are destroyed in reverse, so every
	// GL resource below m_window is released while its context still exists.
	FlyCamera m_camera;
	std::unique_ptr<Window> m_window;

	Scene m_scene;

	Shader m_litShader;     // lit.vert/frag: textured, lit surfaces
	Shader m_unlitShader;   // unlit.vert/frag: unlit light markers

	Box m_cube;
	VertexBuffer m_cubeVbo;
	VertexArray m_cubeVao;
	Material m_crateMaterial;

	Box m_lightCube;
	VertexBuffer m_lightCubeVbo;
	VertexArray m_lightCubeVao;

	Model m_backpack;

	void uploadBox(const Box& box, VertexBuffer& vbo, VertexArray& vao);
	void applyStaticLighting();
	void renderFrame();
	glm::mat4 projectionMatrix() const;
};
