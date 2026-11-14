#include <render/Shader.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
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
			"Shader program link failed (" + vertexPath + ", " + fragmentPath + "):\n" + infoLog);
	}
}

Shader::~Shader()
{
	if (m_id != 0)
	{
		glDeleteProgram(m_id);
	}
}

Shader::Shader(Shader&& other) noexcept
	: m_id(std::exchange(other.m_id, 0)),
	m_uniformLocations(std::move(other.m_uniformLocations))
{}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (this != &other)
	{
		if (m_id != 0)
		{
			glDeleteProgram(m_id);
		}
		m_id = std::exchange(other.m_id, 0);
		m_uniformLocations = std::move(other.m_uniformLocations);
	}
	return *this;
}

std::string Shader::readShaderFromFile(const std::string& shaderPath)
{
	std::ifstream shaderFile(shaderPath);
	if (!shaderFile)
	{
		throw std::runtime_error("Could not open shader file: " + shaderPath);
	}

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	if (shaderFile.bad())
	{
		throw std::runtime_error("Could not read shader file: " + shaderPath);
	}
	return shaderStream.str();
}

unsigned int Shader::compileStage(GLenum stage, const std::string& source, const std::string& path)
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

void Shader::use() const
{
	glUseProgram(m_id);
}

unsigned int Shader::getID() const
{
	return m_id;
}

int Shader::getUniformLocation(const std::string& name) const
{
	auto it = m_uniformLocations.find(name);
	if (it != m_uniformLocations.end())
	{
		return it->second;
	}
	const int location = glGetUniformLocation(m_id, name.c_str());
	m_uniformLocations.emplace(name, location);
	return location;
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(getUniformLocation(name), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
