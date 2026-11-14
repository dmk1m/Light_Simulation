#pragma once

#include <shader/OpenGLShader.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

// A texture referenced by a mesh. The GL texture itself is owned by the
// Model that loaded it (textures are shared between meshes).
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

// Owns the mesh's VAO, VBO and EBO. Move-only: copying would delete the same
// GL objects twice.
class Mesh {
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Draw(const OpenGLShader& shader) const;
private:
    //  render data
    unsigned int VAO = 0, VBO = 0, EBO = 0;

    void setupMesh();
    void release();
};
