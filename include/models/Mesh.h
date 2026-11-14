#pragma once

#include <buffers/IndexBuffer.h>
#include <buffers/VertexArray.h>
#include <buffers/VertexBuffer.h>
#include <buffers/VertexLayout.h>
#include <render/Material.h>
#include <shader/OpenGLShader.h>

#include <glm/glm.hpp>

#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    static VertexLayout layout();
};

// Indexed triangle mesh uploaded to the GPU, with its material.
// Move-only (its buffers are).
class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Material material);

    // The shader must be in use; sets material.* and draws.
    void draw(const OpenGLShader& shader) const;

    const Material& getMaterial() const;

private:
    VertexArray m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ebo;
    Material m_material;
};
