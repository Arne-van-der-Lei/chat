#include "stdafx.h"
#include "TextRenderer.h"
#include "ShaderUtil.h"
#include "Texture.h"

TextRenderer::TextRenderer()
{
	initilizeShader();
	glGenBuffers(1, &m_vert);
	glBindBuffer(GL_ARRAY_BUFFER, m_vert);
	glBufferData(GL_ARRAY_BUFFER, 800*800*2*sizeof(vec3), NULL, GL_DYNAMIC_DRAW);
	m_pTexture = new Texture("Resources/Textures/Glifs.bmp");
}

TextRenderer::~TextRenderer()
{
	m_pShader->Delete();
	delete m_pShader;
	m_pTexture->destroy();
	delete m_pTexture;
}

//adds the text for rendering
void TextRenderer::addText(string str, int x, int y, vec3 color)
{
	for(int i = 0; i < (int)str.length(); i++)
	{
		m_posletters.push_back(vec3((float)(x + i), (float)-y, str[i]));
		m_posletters.push_back(color);
	}
}

//draw the text
void TextRenderer::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vert);
	glBufferSubData(GL_ARRAY_BUFFER,0, m_posletters.size() * sizeof(vec3), m_posletters.data());
	
	m_pShader->Use();
	m_pTexture->use();

	m_pShader->setUniformLocationF("size", 0.04f);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_vert);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3*4));

	glDrawArrays(GL_POINTS, 0, m_posletters.size());

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	m_posletters.clear();
	m_posletters.shrink_to_fit();
}

// creates the text shader
void TextRenderer::initilizeShader()
{
	m_pShader = new ShaderUtil();

	m_pShader->Load("Resources/Shaders/text.vert", "Resources/Shaders/text.geom", "Resources/Shaders/text.frag");
}
