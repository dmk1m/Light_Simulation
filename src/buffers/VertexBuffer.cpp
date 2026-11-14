#include <buffers/VertexBuffer.h>

#include <utility>

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer()
{
    if (m_id != 0)
    {
        glDeleteBuffers(1, &m_id);
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_id(std::exchange(other.m_id, 0))
{}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other)
    {
        if (m_id != 0)
        {
            glDeleteBuffers(1, &m_id);
        }
        m_id = std::exchange(other.m_id, 0);
    }
    return *this;
}

void VertexBuffer::setData(const std::vector<float>& data, GLenum usage)
{
    bind();

    glBufferData(
        GL_ARRAY_BUFFER,
        data.size() * sizeof(float),
        data.data(),
        usage
    );
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int VertexBuffer::getID() const
{
    return m_id;
}
