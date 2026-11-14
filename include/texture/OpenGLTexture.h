#pragma once

class OpenGLTexture {
private:
	int m_width = 0;
	int m_height = 0;
	int m_nrChannels = 0;

	unsigned int m_textureID = 0;

public:
	explicit OpenGLTexture(const char* path);

	int getWidth() const;
	int getHeight() const;
	int getNrChannels() const;
	unsigned int getTextureID() const;

	void bind() const;
	void unbind() const;

};
