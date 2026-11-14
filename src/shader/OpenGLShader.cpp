#include <shader/OpenGLShader.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

OpenGLShader::OpenGLShader(const char* vertexPath, const char* fragmentPath)
{
	const std::string vertexCode = readShaderFromFile(vertexPath);
	const std::string fragmentCode = readShaderFromFile(fragmentPath);

	const unsigned int vertex = compileStage(GL_VERTEX_SHADER, vertexCode, vertexPath);
	unsigned int fragment = 0;
	try
	{
		fragment = compileStage(GL_FRAGMENT_SHADER, fragmentCode, fragmentPath);
	}
	catch (...)
	{
		glDeleteShader(vertex);
		throw;
	}

	m_id = glCreateProgram();
	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);
	glLinkProgram(m_id);

	// The program keeps what it needs; the stage objects can go either way.
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	int success = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(m_id, sizeof(infoLog), nullptr, infoLog);
		glDeleteProgram(m_id);
		m_id = 0;
		throw std::runtime_error(
			std::string("Shader program link failed (") + vertexPath + ", " + fragmentPath + "):\n" + infoLog);
	}
}

OpenGLShader::~OpenGLShader()
{
	if (m_id != 0)
	{
		glDeleteProgram(m_id);
	}
}

OpenGLShader::OpenGLShader(OpenGLShader&& other) noexcept
	: m_id(std::exchange(other.m_id, 0))
{}

OpenGLShader& OpenGLShader::operator=(OpenGLShader&& other) noexcept
{
	if (this != &other)
	{
		if (m_id != 0)
		{
			glDeleteProgram(m_id);
		}
		m_id = std::exchange(other.m_id, 0);
	}
	return *this;
}

std::string OpenGLShader::readShaderFromFile(const char* shaderPath)
{
	std::ifstream shaderFile(shaderPath);
	if (!shaderFile)
	{
		throw std::runtime_error(std::string("Could not open shader file: ") + shaderPath);
	}

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	if (shaderFile.bad())
	{
		throw std::runtime_error(std::string("Could not read shader file: ") + shaderPath);
	}
	return shaderStream.str();
}

unsigned int OpenGLShader::compileStage(GLenum stage, const std::string& source, const char* path)
{
	const unsigned int shader = glCreateShader(stage);
	const char* code = source.c_str();
	glShaderSource(shader, 1, &code, nullptr);
	glCompileShader(shader);

	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
		glDeleteShader(shader);
		const char* stageName = stage == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
		throw std::runtime_error(std::string(stageName) + " shader compile failed (" + path + "):\n" + infoLog);
	}
	return shader;
}

void OpenGLShader::use() const
{
	glUseProgram(m_id);
}

unsigned int OpenGLShader::getID() const
{
	return m_id;
}

void OpenGLShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void OpenGLShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void OpenGLShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void OpenGLShader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
}

void OpenGLShader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
}

void OpenGLShader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void OpenGLShader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
