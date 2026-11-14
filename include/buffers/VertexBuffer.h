#pragma once

#include <glad/glad.h>
#include <vector>

// Owns one OpenGL buffer object. Move-only: copying would delete the same
// buffer twice.
class VertexBuffer {
private:
    unsigned int m_id = 0;

public:
    VertexBuffer();
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void setData(const std::vector<float>& data, GLenum usage = GL_STATIC_DRAW);

    void bind() const;
    void unbind() const;

    unsigned int getID() const;
};
