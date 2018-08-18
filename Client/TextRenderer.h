#pragma once
class ShaderUtil;
class Texture;
struct vec3
{
	vec3(float x,float y,float z) :x(x),y(y),z(z) {}
	float x;
	float y;
	float z;
};

struct vec2
{
	float x;
	float y;
	float z;
};

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();


	//adds text to the renderer
	void addText(string str, int x, int y, vec3 color = vec3(1.f,1.f,1.f));

	//renders the text
	void Draw();

	// Shaders
	void initilizeShader();

private:
	//shader
	ShaderUtil * m_pShader;

	//data
	vector<vec3> m_colorletters;
	vector<vec3> m_posletters;

	//buffers
	GLuint m_vert;


	//texture
	Texture* m_pTexture;
};

