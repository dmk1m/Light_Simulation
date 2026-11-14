#include <geometry/Model.h>

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
	// Path of the first texture of the given type, or empty if there is none.
	std::string firstTexturePath(const aiMaterial* material, aiTextureType type)
	{
		if (material->GetTextureCount(type) == 0)
		{
			return {};
		}
		aiString path;
		material->GetTexture(type, 0, &path);
		return path.C_Str();
	}
}

Model::Model(const std::filesystem::path& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		path.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::runtime_error("Failed to load model '" + path.string() + "': " + importer.GetErrorString());
	}

	m_directory = path.parent_path();
	processNode(scene->mRootNode, scene);
}

void Model::draw(const Shader& shader) const
{
	for (const Mesh& mesh : m_meshes)
	{
		mesh.draw(shader);
	}
}

void Model::processNode(const aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		m_meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		// aiProcess_GenSmoothNormals fills these in, but points/lines have none.
		vertex.Normal = mesh->HasNormals()
			? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
			: glm::vec3(0.0f, 1.0f, 0.0f);

		vertex.TexCoords = mesh->HasTextureCoords(0)
			? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
			: glm::vec2(0.0f);

		vertices.push_back(vertex);
	}

	std::vector<unsigned int> indices;
	indices.reserve(static_cast<std::size_t>(mesh->mNumFaces) * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
	}

	Material material;
	if (mesh->mMaterialIndex < scene->mNumMaterials)
	{
		material = loadMaterial(scene->mMaterials[mesh->mMaterialIndex]);
	}

	return Mesh(vertices, indices, std::move(material));
}

Material Model::loadMaterial(const aiMaterial* aiMat)
{
	Material material;
	material.diffuse = loadTexture(firstTexturePath(aiMat, aiTextureType_DIFFUSE));
	material.specular = loadTexture(firstTexturePath(aiMat, aiTextureType_SPECULAR));

	float shininess = 0.0f;
	if (aiMat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS && shininess > 0.0f)
	{
		material.shininess = shininess;
	}
	return material;
}

std::shared_ptr<const Texture> Model::loadTexture(const std::string& relativePath)
{
	if (relativePath.empty())
	{
		return nullptr;
	}

	auto cached = m_textureCache.find(relativePath);
	if (cached != m_textureCache.end())
	{
		return cached->second;
	}

	std::shared_ptr<const Texture> texture;
	try
	{
		texture = std::make_shared<const Texture>((m_directory / relativePath).string());
	}
	catch (const std::exception& e)
	{
		// A missing map shouldn't stop the model loading; it renders without it.
		std::cerr << "Warning: " << e.what() << std::endl;
	}
	m_textureCache.emplace(relativePath, texture);
	return texture;
}
