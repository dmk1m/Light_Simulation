#pragma once

#include <models/Mesh.h>
#include <shader/OpenGLShader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

// Loads a model with Assimp. Owns its meshes and every texture it loaded
// (meshes share textures by id). Move-only. The constructor throws
// std::runtime_error if the file can't be loaded.
class Model {
public:
	explicit Model(const char* path);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	Model(Model&& other) noexcept;
	Model& operator=(Model&& other) noexcept;

	void Draw(const OpenGLShader& shader) const;
private:
	std::vector<Texture> m_textures_loaded;
	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);
	void releaseTextures();
};
