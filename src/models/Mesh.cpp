#include <models/Mesh.h>

#include <glad/glad.h>

#include <cstddef>
#include <utility>
#include <vector>

VertexLayout Vertex::layout()
{
    return VertexLayout{
        {
            { AttribLocation::Position, 3, offsetof(Vertex, Position) },
            { AttribLocation::TexCoord, 2, offsetof(Vertex, TexCoords) },
            { AttribLocation::Normal,   3, offsetof(Vertex, Normal) },
        },
        sizeof(Vertex)
    };
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Material material)
    : m_material(std::move(material))
{
    m_vao.bind();
    m_vbo.setData(vertices);
    m_vao.setLayout(Vertex::layout());
    m_ebo.setData(indices);     // recorded in the bound VAO
    m_vao.unbind();
}

void Mesh::draw(const OpenGLShader& shader) const
{
    m_material.apply(shader);

    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_ebo.getCount(), GL_UNSIGNED_INT, nullptr);
    m_vao.unbind();
}

const Material& Mesh::getMaterial() const
{
    return m_material;
}
