#include <render/IndexBuffer.h>

#include <utility>

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &m_id);
}

IndexBuffer::~IndexBuffer()
{
	if (m_id != 0)
	{
		glDeleteBuffers(1, &m_id);
	}
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	: m_id(std::exchange(other.m_id, 0)),
	m_count(std::exchange(other.m_count, 0))
{}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
	if (this != &other)
	{
		if (m_id != 0)
		{
			glDeleteBuffers(1, &m_id);
		}
		m_id = std::exchange(other.m_id, 0);
		m_count = std::exchange(other.m_count, 0);
	}
	return *this;
}

void IndexBuffer::setData(const std::vector<unsigned int>& indices, GLenum usage)
{
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);
	m_count = static_cast<GLsizei>(indices.size());
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

GLsizei IndexBuffer::getCount() const
{
	return m_count;
}

unsigned int IndexBuffer::getID() const
{
	return m_id;
}
