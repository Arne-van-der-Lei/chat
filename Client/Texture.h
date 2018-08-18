#pragma once
class Texture
{
public:
	Texture(string path);
	~Texture();

	void use();
	void destroy();
private:
	GLuint m_texture;
};

