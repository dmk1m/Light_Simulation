#pragma once

#include <geometry/Mesh.h>
#include <render/Shader.h>
#include <render/Texture.h>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

// Loads a model with Assimp into GPU meshes. Textures are loaded once and
// shared between meshes. Move-only. The constructor throws
// std::runtime_error if the file can't be loaded.
class Model {
public:
	explicit Model(const std::filesystem::path& path);

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	Model(Model&&) noexcept = default;
	Model& operator=(Model&&) noexcept = default;

	// The shader must be in use; draws every mesh with its own material.
	void draw(const Shader& shader) const;

private:
	std::vector<Mesh> m_meshes;
	std::filesystem::path m_directory;
	std::unordered_map<std::string, std::shared_ptr<const Texture>> m_textureCache;

	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
	Material loadMaterial(const aiMaterial* material);
	std::shared_ptr<const Texture> loadTexture(const std::string& relativePath);
};
