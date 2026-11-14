#include <render/VertexArray.h>

#include <utility>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    if (m_id != 0)
    {
        glDeleteVertexArrays(1, &m_id);
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_id(std::exchange(other.m_id, 0))
{}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        if (m_id != 0)
        {
            glDeleteVertexArrays(1, &m_id);
        }
        m_id = std::exchange(other.m_id, 0);
    }
    return *this;
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

unsigned int VertexArray::getID() const
{
    return m_id;
}

void VertexArray::setLayout(const VertexLayout& layout)
{
    bind();

    const GLsizei stride = static_cast<GLsizei>(layout.strideBytes);
    for (const VertexAttributeDesc& attribute : layout.attributes)
    {
        glVertexAttribPointer(
            attribute.location,
            attribute.components,
            GL_FLOAT,
            GL_FALSE,
            stride,
            reinterpret_cast<void*>(attribute.offsetBytes)
        );
        glEnableVertexAttribArray(attribute.location);
    }
}
