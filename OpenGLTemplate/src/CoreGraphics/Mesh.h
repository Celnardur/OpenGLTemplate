#ifndef MESH_H
#define MESH_H

#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"


class Mesh
{
public:
	Mesh();

	template<class Vertex>
	void create(const VertexData<Vertex> & vertices, const Texture & texture = {},
		bool bUseBlending = false);

	void render();
	void destroy();

	// Uniform Functions
	void setColor(float r, float g, float b, float a = 1.0f);

private:
	VertexArray m_vao;
	Texture m_texture;
	Shader m_shader;

	bool m_bUseBlending;
};

template <class Vertex>
void Mesh::create(const VertexData<Vertex> & vertices, const Texture & texture,
	bool bUseBlending)
{
	m_vao.create<Vertex>(vertices);
	m_texture = texture;
	m_shader = Shader(vertices.aDimensions);
	m_bUseBlending = bUseBlending;
}

#endif
