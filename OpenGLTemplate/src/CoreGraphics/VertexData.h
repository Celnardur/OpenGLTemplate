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
};

struct ColorA
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
	SPECULAR,
	LAST
};

// This contains all the data necessary to construct a VAO (VertexArray).
// Data will be added through this class and this class will be
// used to construct VertexArrays and Meshes.
class VertexData
{
public:
	VertexData();
	VertexData(Attribute iAttribute, const vector<Point2D> & vPos);
	VertexData(Attribute iAttribute, const vector<Point3D> & vPos);

	VertexData(const VertexData & toCopy);
	VertexData(VertexData && toCopy) noexcept;
	VertexData& operator=(VertexData toCopy);
	~VertexData();

	friend void swap(VertexData & first, VertexData & second) noexcept;

	void add(Attribute iAttribute, const vector<float> & vfPoints, int nDimension);
	void add(Attribute iAttribute, const vector<Point2D> & vPos);
	void add(Attribute iAttribute, const vector<Point3D> & vPos);
	void add(Attribute iAttribute, const vector<Color> & vPos);
	void add(Attribute iAttribute, const vector<ColorA> & vPos);

	void remove(Attribute iAttribute);

	void clear();

	int getStride() const;
	int getOffset(Attribute iAttribute) const;
	void getDimensions(int pDimensions[LAST]) const;
	vector<float> * synthesize() const;
	void synthesize(vector<float> * vfVertices) const;

	void test();

private:
	vector<float> (* m_vfVertices)[LAST];
	vector<unsigned int> m_vuiIndices;

	int m_pDimensions[LAST];
	size_t m_nVertices;
};

#endif
