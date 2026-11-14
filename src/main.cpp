#include <core/Color.h>
#include <shapes/Box.h>
#include <window/Window.h>
#include <window/GLFWindow.h>
#include <camera/Camera.h>
#include <camera/OpenGLCamera.h>
#include <buffers/VertexBuffer.h>
#include <buffers/VertexArray.h>
#include <texture/OpenGLTexture.h>
#include <models/Model.h>
#include <shader/OpenGLShader.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <exception>
#include <iostream>
#include <memory>

const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

static int run() {

	OpenGLCamera camera(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	// The window owns the OpenGL context, so it must outlive every GL object below.
	std::unique_ptr<Window> window = std::make_unique<GLFWindow>(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window");
	window->setCamera(camera);
	window->lockCursor();

	{
		OpenGLShader boxShader("shaders/cube.vert", "shaders/cube.frag");
		OpenGLShader lightShader("shaders/light.vert", "shaders/light.frag");

		Box box(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, Color::White);
		box.includeTextureCoordinates();
		box.includeNormal();

		Box lightBox(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, Color::White);

		OpenGLTexture diffuseMap("textures/container2.png");
		OpenGLTexture specularMap("textures/container2_specular.png");

		VertexBuffer VBO;
		VBO.bind();
		VBO.setData(box.getVertices());

		VertexArray VAO;
		VAO.bind();
		VAO.setLayout(box);

		VBO.unbind();
		VAO.unbind();

		VertexBuffer lightVBO;
		lightVBO.bind();
		lightVBO.setData(lightBox.getVertices());

		VertexArray lightVAO;
		lightVAO.bind();
		lightVAO.setLayout(lightBox);

		lightVBO.unbind();
		lightVAO.unbind();

		glm::vec3 cubePositions[] = {
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

		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
		};

		Model backpack("models/backpack/backpack.obj");
		const glm::vec3 backpackPosition(0.0f, 0.0f, -6.0f);
		const float backpackScale = 0.5f;

		while (!window->shouldClose())
		{
			window->processInput();
			window->setBackgroundColor(Color::Gray);
			window->clearDepthBuffer();

			boxShader.use();

			boxShader.setVec3("viewPos", camera.getPosition());

			// Set material properties
			boxShader.setInt("material.diffuse", 0);
			boxShader.setInt("material.specular", 1);
			boxShader.setFloat("material.shininess", 64.0f);

			// directional light
			boxShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			boxShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			boxShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			boxShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

			// point light 1
			boxShader.setVec3("pointLights[0].position", pointLightPositions[0]);
			boxShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			boxShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			boxShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			boxShader.setFloat("pointLights[0].constant", 1.0f);
			boxShader.setFloat("pointLights[0].linear", 0.09f);
			boxShader.setFloat("pointLights[0].quadratic", 0.032f);
			// point light 2
			boxShader.setVec3("pointLights[1].position", pointLightPositions[1]);
			boxShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			boxShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			boxShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
			boxShader.setFloat("pointLights[1].constant", 1.0f);
			boxShader.setFloat("pointLights[1].linear", 0.09f);
			boxShader.setFloat("pointLights[1].quadratic", 0.032f);
			// point light 3
			boxShader.setVec3("pointLights[2].position", pointLightPositions[2]);
			boxShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
			boxShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
			boxShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
			boxShader.setFloat("pointLights[2].constant", 1.0f);
			boxShader.setFloat("pointLights[2].linear", 0.09f);
			boxShader.setFloat("pointLights[2].quadratic", 0.032f);
			// point light 4
			boxShader.setVec3("pointLights[3].position", pointLightPositions[3]);
			boxShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
			boxShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
			boxShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
			boxShader.setFloat("pointLights[3].constant", 1.0f);
			boxShader.setFloat("pointLights[3].linear", 0.09f);
			boxShader.setFloat("pointLights[3].quadratic", 0.032f);
			// spotLight
			boxShader.setVec3("spotLight.position", camera.getPosition());
			boxShader.setVec3("spotLight.direction", camera.getFront());
			boxShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			boxShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			boxShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			boxShader.setFloat("spotLight.constant", 1.0f);
			boxShader.setFloat("spotLight.linear", 0.09f);
			boxShader.setFloat("spotLight.quadratic", 0.032f);
			boxShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			boxShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

			// View
			glm::mat4 view = camera.getViewMatrix();
			boxShader.setMat4("view", view);

			// Projection (use the live framebuffer size; height is 0 while minimized)
			int fbWidth = window->getWidth();
			int fbHeight = window->getHeight();
			float aspect = fbHeight > 0 ? static_cast<float>(fbWidth) / static_cast<float>(fbHeight) : 1.0f;
			glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), aspect, 0.1f, 100.0f);
			boxShader.setMat4("projection", projection);

			// Set textures
			glActiveTexture(GL_TEXTURE0);
			diffuseMap.bind();
			glActiveTexture(GL_TEXTURE1);
			specularMap.bind();

			VAO.bind();
			for (const glm::vec3& position : cubePositions)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
				boxShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, box.getVertexCount());
			}

			// Backpack model (uses the same lighting shader)
			glm::mat4 backpackModel = glm::translate(glm::mat4(1.0f), backpackPosition);
			backpackModel = glm::scale(backpackModel, glm::vec3(backpackScale));
			boxShader.setMat4("model", backpackModel);
			backpack.Draw(boxShader);

			// Light source cubes
			lightShader.use();
			lightShader.setMat4("projection", projection);
			lightShader.setMat4("view", view);
			lightShader.setVec3("lightColor", lightBox.getColorVec3());

			lightVAO.bind();
			for (const glm::vec3& position : pointLightPositions)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
				model = glm::scale(model, glm::vec3(0.2f));
				lightShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, lightBox.getVertexCount());
			}

			window->update();
		}
	}

	return 0;
}

int main() {
	try
	{
		return run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return 1;
	}
}
