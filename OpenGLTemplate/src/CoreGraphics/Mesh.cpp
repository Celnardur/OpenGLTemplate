#include "Mesh.h"

Mesh::Mesh()
{
	m_bUseBlending = false;
}

void Mesh::create(VertexArray vao, std::vector<Texture> textures, bool bUseBlending)
{
	myAssert(textures.size() <= 16, "Cannot have more than 16 textures.");
	m_vao = vao;
	m_textures = textures;
	m_bUseBlending = bUseBlending;
}

void Mesh::render()
{
	if (m_bUseBlending)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	m_vao.bind();
	for (int i = 0; i < m_textures.size(); ++i)
		m_textures[i].activate(i);

	m_vao.render();

	m_vao.unbind();

	if (m_bUseBlending)
		glDisable(GL_BLEND);
}

void Mesh::destroy()
{
	m_vao.destroy();
	for (Texture e : m_textures)
		e.destroy();
}
