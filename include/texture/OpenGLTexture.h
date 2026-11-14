#pragma once

#include <string>

// Owns one OpenGL 2D texture loaded from an image file (mipmapped, repeat
// wrapping). Move-only: copying would delete the same texture twice.
// The constructor throws std::runtime_error if the image can't be loaded.
class OpenGLTexture {
private:
	int m_width = 0;
	int m_height = 0;
	int m_nrChannels = 0;

	unsigned int m_textureID = 0;

public:
	explicit OpenGLTexture(const std::string& path);
	~OpenGLTexture();

	OpenGLTexture(const OpenGLTexture&) = delete;
	OpenGLTexture& operator=(const OpenGLTexture&) = delete;
	OpenGLTexture(OpenGLTexture&& other) noexcept;
	OpenGLTexture& operator=(OpenGLTexture&& other) noexcept;

	int getWidth() const;
	int getHeight() const;
	int getNrChannels() const;
	unsigned int getTextureID() const;

	void bind() const;
	// Binds to texture unit GL_TEXTURE0 + unit (leaves that unit active).
	void bindToUnit(unsigned int unit) const;
	void unbind() const;

};
