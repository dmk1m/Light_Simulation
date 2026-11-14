#pragma once
#include <buffers/VertexLayout.h>
#include <core/Color.h>
#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

enum class VertexAttribute {
	Position = 1 << 0,
	Color = 1 << 1,
	TextureCoordinate = 1 << 2,
	Normal = 1 << 3
};

// CPU-side geometry for a box centered on the origin (36 vertices, no
// indices). Place it in the world with a model matrix. Optional per-vertex
// attributes (color, texture coordinates, normals) can be switched on.
class Box {
private:
	float m_width;
	float m_height;
	float m_depth;

	Color m_color;

	unsigned int m_attributes;
	std::size_t m_strideFloats;

	std::vector<float> m_vertices;
private:
	void calculateStride();

	void addVertex(
		const glm::vec3& position,
		const glm::vec2& texCoord,
		const glm::vec3& normal
	);

	void addFace(
		const glm::vec3& v0,
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const glm::vec3& normal
	);

	void buildVertices();
public:
	explicit Box(float width = 1.0f, float height = 1.0f, float depth = 1.0f, Color color = Color::White);

	float getWidth() const;
	float getHeight() const;
	float getDepth() const;

	void setWidth(float width);
	void setHeight(float height);
	void setDepth(float depth);

	Color getColor() const;
	glm::vec3 getColorVec3() const;
	void setColor(const Color& color);

	void includeColor();
	void excludeColor();

	void includeTextureCoordinates();
	void excludeTextureCoordinates();

	void includeNormal();
	void excludeNormal();

	bool hasAttribute(VertexAttribute attribute) const;
	bool hasColor() const;
	bool hasTextureCoordinate() const;
	bool hasNormal() const;

	const std::vector<float>& getVertices() const;
	std::size_t getStrideFloats() const;
	std::size_t getStrideBytes() const;
	int getVertexCount() const;

	// Layout of getVertices(), for VertexArray::setLayout.
	VertexLayout getLayout() const;

	std::size_t getPositionOffsetBytes() const;
	std::size_t getColorOffsetBytes() const;
	std::size_t getTextureCoordinatesOffsetBytes() const;
	std::size_t getNormalOffsetBytes() const;

};
