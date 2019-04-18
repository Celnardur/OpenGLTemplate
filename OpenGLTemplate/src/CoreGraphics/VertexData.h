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
// The api is the top priority followed by efficiency b/c this is
// a loading task.
// The template parameter is assumed to be a tightly packed structure
// with a copy constructor (no pointers).
// This class assumes you know what your doing.
template<class Vertex>
struct VertexData
{
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_vuiIndices;

	unsigned int m_pDimensions[LAST];

	std::string strTexture;
	std::string strSpecularTexture;

	VertexData(std::vector<unsigned int> viDimensions);

	int getStride() const;
	int getOffset(Attribute iAttribute) const;

	void test();
};

template <class Vertex>
VertexData<Vertex>::VertexData(std::vector<unsigned int> viDimensions)
{
	myAssert(viDimensions.size() <= LAST, "Tried to create too many attributes.");
	for(int i = 0; i < viDimensions.size(); ++i)
	{
		m_pDimensions[i] = viDimensions[i];
	}
}

template <class Vertex>
int VertexData<Vertex>::getStride() const
{
	int nSum = 0;
	for (int e : m_pDimensions)
	{
		nSum += e;
	}
	return nSum;
}

template <class Vertex>
int VertexData<Vertex>::getOffset(Attribute iAttribute) const
{
	int iOffset = 0;
	for (int i = 0; i < iAttribute; ++i)
	{
		iOffset += m_pDimensions[i];
	}
	return iOffset;
}

#endif
