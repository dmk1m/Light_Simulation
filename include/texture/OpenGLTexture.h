#pragma once

// Owns one OpenGL 2D texture. Move-only: copying would delete the same
// texture twice.
class OpenGLTexture {
private:
	int m_width = 0;
	int m_height = 0;
	int m_nrChannels = 0;

	unsigned int m_textureID = 0;

public:
	explicit OpenGLTexture(const char* path);
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
	void unbind() const;

};
