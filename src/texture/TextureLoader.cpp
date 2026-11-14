#include <texture/OpenGLTexture.h>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <stdexcept>
#include <string>
#include <utility>

OpenGLTexture::OpenGLTexture(const char* path)
{
	unsigned char* data = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);
	if (!data)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + path);
	}

	GLenum format;
	switch (m_nrChannels)
	{
	case 1: format = GL_RED;  break;
	case 2: format = GL_RG;   break;
	case 3: format = GL_RGB;  break;
	case 4: format = GL_RGBA; break;
	default:
		stbi_image_free(data);
		throw std::runtime_error(std::string("Unsupported texture channel count: ") + path);
	}

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Rows of RGB/RG/R images are not necessarily 4-byte aligned.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

OpenGLTexture::~OpenGLTexture()
{
	if (m_textureID != 0)
	{
		glDeleteTextures(1, &m_textureID);
	}
}

OpenGLTexture::OpenGLTexture(OpenGLTexture&& other) noexcept
	: m_width(std::exchange(other.m_width, 0)),
	m_height(std::exchange(other.m_height, 0)),
	m_nrChannels(std::exchange(other.m_nrChannels, 0)),
	m_textureID(std::exchange(other.m_textureID, 0))
{}

OpenGLTexture& OpenGLTexture::operator=(OpenGLTexture&& other) noexcept
{
	if (this != &other)
	{
		if (m_textureID != 0)
		{
			glDeleteTextures(1, &m_textureID);
		}
		m_width = std::exchange(other.m_width, 0);
		m_height = std::exchange(other.m_height, 0);
		m_nrChannels = std::exchange(other.m_nrChannels, 0);
		m_textureID = std::exchange(other.m_textureID, 0);
	}
	return *this;
}

int OpenGLTexture::getWidth() const
{
	return m_width;
}

int OpenGLTexture::getHeight() const
{
	return m_height;
}

int OpenGLTexture::getNrChannels() const
{
	return m_nrChannels;
}

unsigned int OpenGLTexture::getTextureID() const
{
	return m_textureID;
}

void OpenGLTexture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void OpenGLTexture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
