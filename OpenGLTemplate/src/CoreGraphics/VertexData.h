#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include <vector>
using std::vector;

struct Point2D
{
	float x, y;
};

struct Point3D
{
	float x, y, z;
};

struct Color
{
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 1;
};

enum Attribute
{
	POSITION,
	COLOR,
	TEXTURE,
	NORMAL,
	SPECULAR
};

// This contains all the data necessary to construct a VAO (VertexArray).
// Data will be added through this class and this class will be
// used to construct VertexArrays and Meshes.
class VertexData
{
public:
	VertexData();

	~VertexData() = default;

private:
	vector<float> m_vVertices;
	vector<unsigned int> m_vuiIndices;

	int m_pDimensions[5];
};

#endif
