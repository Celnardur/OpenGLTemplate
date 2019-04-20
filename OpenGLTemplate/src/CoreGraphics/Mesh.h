#ifndef MESH_H
#define MESH_H

#include "VertexArray.h"
#include "Texture.h"


class Mesh
{
public:
	Mesh();

	template<class Vertex>
	void create(VertexData<Vertex> vertices, std::vector<Texture> textures = {},
		bool bUseBlending = false);

	void create(VertexArray vao, std::vector<Texture> textures = {},
		bool bUseBlending = false);

	void render();
	void destroy();

private:
	VertexArray m_vao;
	std::vector<Texture> m_textures;

	bool m_bUseBlending;
};

template <class Vertex>
void Mesh::create(VertexData<Vertex> vertices, std::vector<Texture> textures, bool bUseBlending)
{
	myAssert(textures.size() <= 16, "Cannot have more than 16 textures.");
	m_vao.create<Vertex>(vertices);
	m_textures = textures;
	m_bUseBlending = bUseBlending;
}

#endif
