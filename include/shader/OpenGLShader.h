#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

// Owns one linked OpenGL shader program. Move-only: copying would delete the
// same program twice. The constructor throws std::runtime_error if a source
// file can't be read or the shaders fail to compile or link.
class OpenGLShader {
private:
    unsigned int m_id = 0;

    // Uniform name -> location, filled on first use (-1 for unknown names).
    mutable std::unordered_map<std::string, int> m_uniformLocations;

    static std::string readShaderFromFile(const std::string& shaderPath);
    static unsigned int compileStage(GLenum stage, const std::string& source, const std::string& path);

    int getUniformLocation(const std::string& name) const;

public:
    OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
    ~OpenGLShader();

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&& other) noexcept;
    OpenGLShader& operator=(OpenGLShader&& other) noexcept;

    // use/activate the shader
    void use() const;
    unsigned int getID() const;

    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};
