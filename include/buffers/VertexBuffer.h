#pragma once

#include <glad/glad.h>
#include <cstddef>
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

    void setData(const void* data, std::size_t sizeBytes, GLenum usage = GL_STATIC_DRAW);

    template <typename T>
    void setData(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW)
    {
        setData(data.data(), data.size() * sizeof(T), usage);
    }

    void bind() const;
    void unbind() const;

    unsigned int getID() const;
};
