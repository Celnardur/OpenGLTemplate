#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include <vector>
#include "../utils.h"

enum Attribute
{
	POSITION,
	COLOR,
	TEXTURE,
	NORMAL,
	SPECULAR,
	LAST
};

// This contains all the data necessary to construct a VAO (VertexArray).
// Data will be added through this class and this class will be
// used to construct VertexArrays and Meshes.
// This was designed with procedural generation in mind.
// The api is the top priority followed by efficiency b/c this is a loading task.
// The template parameter is assumed to be a tightly packed structure of floats.
// This struct assumes you know what your doing.
template<class Vertex>
struct VertexData
{
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_vuiIndices;

	unsigned int m_pDimensions[LAST];

	VertexData(std::vector<unsigned int> viDimensions);

	int getStride() const;
	int getOffset(Attribute iAttribute) const;

};

template <class Vertex>
VertexData<Vertex>::VertexData(std::vector<unsigned int> viDimensions)
{
	myAssert(viDimensions.size() <= LAST, "Tried to create too many attributes.");

	for(int i = 0; i < LAST; ++i)
	{
		if (i < viDimensions.size())
			m_pDimensions[i] = viDimensions[i];
		else
			m_pDimensions[i] = 0;
	}
}

template <class Vertex>
int VertexData<Vertex>::getStride() const
{
	return sizeof(Vertex);
}

template <class Vertex>
int VertexData<Vertex>::getOffset(Attribute iAttribute) const
{
	int iOffset = 0;
	for (int i = 0; i < iAttribute; ++i)
	{
		iOffset += m_pDimensions[i];
	}
	return iOffset * sizeof(float);
}

#endif
