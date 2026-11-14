#pragma once

#include <buffers/VertexArray.h>
#include <buffers/VertexBuffer.h>
#include <camera/OpenGLCamera.h>
#include <models/Model.h>
#include <render/Material.h>
#include <scene/Scene.h>
#include <shader/OpenGLShader.h>
#include <shapes/Box.h>
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
	OpenGLCamera m_camera;
	std::unique_ptr<Window> m_window;

	Scene m_scene;

	OpenGLShader m_litShader;     // cube.vert/frag: textured, lit surfaces
	OpenGLShader m_lightShader;   // light.vert/frag: unlit light markers

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
