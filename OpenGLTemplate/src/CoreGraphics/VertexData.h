#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include <vector>
#include <array>

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
	std::vector<Vertex> vertices;
	std::vector<unsigned int> vuiIndices;

	std::array<unsigned int, LAST> aDimensions;

	VertexData(const std::vector<unsigned int> & viDimensions);

	int getStride() const;
	int getOffset(Attribute iAttribute) const;

	uint64_t hashVertices();
	uint64_t hashIndices();
};

template <class Vertex>
VertexData<Vertex>::VertexData(const std::vector<unsigned int> & viDimensions)
{
	for(unsigned int i = 0; i < LAST; ++i)
	{
		if (i < viDimensions.size())
			aDimensions[i] = viDimensions[i];
		else
			aDimensions[i] = 0;
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
		iOffset += aDimensions[i];
	}
	return iOffset * sizeof(float);
}

#define FNV_OFFSET_BASIS (0xcbf29ce484222325ULL)
#define FNV_PRIME (0x100000001b3ULL)

template <class Vertex>
uint64_t VertexData<Vertex>::hashVertices()
{
	uint64_t hash = FNV_OFFSET_BASIS;
	char * data = (char*)&(vertices[0]);
	for (int i = 0; i < vertices.size() * sizeof(Vertex); ++i)
	{
		hash ^= data[i];
		hash *= FNV_PRIME;
	}

	return hash;
}

template <class Vertex>
uint64_t VertexData<Vertex>::hashIndices()
{
	uint64_t hash = FNV_OFFSET_BASIS;
	char * data = (char*)&(vuiIndices[0]);
	for (int i = 0; i < vuiIndices.size() * sizeof(vuiIndices); ++i)
	{
		hash ^= data[i];
		hash *= FNV_PRIME;
	}

	return hash;

}

#endif
