#pragma once

#include <glad/glad.h>
#include <buffers/VertexLayout.h>

// Owns one OpenGL vertex array object. Move-only: copying would delete the
// same VAO twice.
class VertexArray {
private:
    unsigned int m_id = 0;

public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void bind() const;
    void unbind() const;

    // Binds this VAO and records the layout for the currently bound
    // GL_ARRAY_BUFFER.
    void setLayout(const VertexLayout& layout);

    unsigned int getID() const;
};
