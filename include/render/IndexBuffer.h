#pragma once

#include <glad/glad.h>
#include <vector>

// Owns one OpenGL element (index) buffer. Move-only. Bind it while the
// vertex array that should use it is bound.
class IndexBuffer {
private:
	unsigned int m_id = 0;
	GLsizei m_count = 0;

public:
	IndexBuffer();
	~IndexBuffer();

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;
	IndexBuffer(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;

	void setData(const std::vector<unsigned int>& indices, GLenum usage = GL_STATIC_DRAW);

	void bind() const;

	GLsizei getCount() const;
	unsigned int getID() const;
};
