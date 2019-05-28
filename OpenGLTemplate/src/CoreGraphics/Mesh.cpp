#include "Mesh.h"

Mesh::Mesh()
{
	m_bUseBlending = false;
}

void Mesh::render()
{
	m_shader.use();
	if (m_bUseBlending)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	m_vao.bind();
	m_texture.activate(0);

	m_vao.render();

	m_vao.unbind();

	if (m_bUseBlending)
		glDisable(GL_BLEND);
}

void Mesh::destroy()
{
	m_vao.destroy();
	m_texture.destroy();
}

void Mesh::setColor(float r, float g, float b, float a)
{
	m_shader.use();
	m_shader.setUniform("color", r, g, b, a);
}

