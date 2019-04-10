#include "VertexData.h"
#include <cassert>
#include <iostream>
using std::cout;
using std::endl;

VertexData::VertexData()
{
	m_vfVertices = nullptr;
	for (int & e : m_pDimensions)
		e = 0;
	m_nVertices = 0;
}

VertexData::VertexData(Attribute iAttribute, const vector<Point2D>& vPos) : VertexData()
{
	this->add(iAttribute, vPos);
}

VertexData::VertexData(Attribute iAttribute, const vector<Point3D>& vPos) : VertexData()
{
	this->add(iAttribute, vPos);
}


VertexData::VertexData(const VertexData& toCopy)
{
	for (int i = 0; i < LAST; ++i)
		m_pDimensions[i] = toCopy.m_pDimensions[i];

	m_vuiIndices = toCopy.m_vuiIndices;
	m_nVertices = toCopy.m_nVertices;

	if (m_nVertices > 0)
	{
		m_vfVertices = new vector<float>[m_nVertices][LAST];
		for (unsigned int iPoint = 0; iPoint < m_nVertices; ++iPoint)
		{
			for (int iAttribute  = 0; iAttribute < LAST; ++iAttribute)
			{
				m_vfVertices[iPoint][iAttribute] = toCopy.m_vfVertices[iPoint][iAttribute];
			}
		}
	}

}

VertexData::VertexData(VertexData&& toCopy) noexcept : VertexData()
{
	swap(*this, toCopy);
}

VertexData& VertexData::operator=(VertexData toCopy)
{
	swap(*this, toCopy);
	return *this;
}

VertexData::~VertexData()
{
	delete[] m_vfVertices;
}

void swap(VertexData& first, VertexData& second) noexcept
{
	using std::swap;

	swap(first.m_vfVertices, second.m_vfVertices);
	swap(first.m_vuiIndices, second.m_vuiIndices);
	swap(first.m_pDimensions, second.m_pDimensions);
	swap(first.m_nVertices, second.m_nVertices);
}



void VertexData::add(Attribute iAttribute, const vector<float>& vfPoints, int nDimension)
{
	assert(iAttribute < LAST && iAttribute >= 0 && "Invalid Attribute.");

	if (m_vfVertices == nullptr)
	{
		m_nVertices = vfPoints.size() / nDimension;
		m_vfVertices = new vector<float>[m_nVertices][LAST];
	}

	assert(m_nVertices == vfPoints.size() / nDimension && "Incorect Number of Vertices");

	int iCursor = 0;
	for (unsigned int i = 0; i < m_nVertices; ++i)
	{
		m_vfVertices[i][iAttribute].reserve(nDimension);
		for (int iPoint = 0; iPoint < nDimension; ++iPoint, ++iCursor)
			m_vfVertices[i][iAttribute].push_back(vfPoints[iCursor]);
	}

	m_pDimensions[iAttribute] = nDimension;
}

void VertexData::add(Attribute iAttribute, const vector<Point2D>& vPos)
{
	assert(iAttribute < LAST && iAttribute >= 0 && "Invalid Attribute.");

	if (m_vfVertices == nullptr)
	{
		m_nVertices = vPos.size();
		m_vfVertices = new vector<float>[vPos.size()][LAST];
	}

	assert(m_nVertices == vPos.size() && "Incorect Number of Vertices");

	for (unsigned int i = 0; i < m_nVertices; ++i)
	{
		m_vfVertices[i][iAttribute].reserve(2);
		m_vfVertices[i][iAttribute].push_back(vPos[i].x);
		m_vfVertices[i][iAttribute].push_back(vPos[i].y);
	}

	m_pDimensions[iAttribute] = 2;
}

void VertexData::add(Attribute iAttribute, const vector<Point3D>& vPos)
{
	assert(iAttribute < LAST && iAttribute >= 0 && "Invalid Attribute.");

	if (m_vfVertices == nullptr)
	{
		m_nVertices = vPos.size();
		m_vfVertices = new vector<float>[vPos.size()][LAST];
	}

	assert(m_nVertices == vPos.size() && "Incorect Number of Vertices");

	for (unsigned int i = 0; i < m_nVertices; ++i)
	{
		m_vfVertices[i][iAttribute].reserve(3);
		m_vfVertices[i][iAttribute].push_back(vPos[i].x);
		m_vfVertices[i][iAttribute].push_back(vPos[i].y);
		m_vfVertices[i][iAttribute].push_back(vPos[i].z);
	}

	m_pDimensions[iAttribute] = 3;
}

void VertexData::add(Attribute iAttribute, const vector<Color>& vPos)
{
	assert(iAttribute < LAST && iAttribute >= 0 && "Invalid Attribute.");

	if (m_vfVertices == nullptr)
	{
		m_nVertices = vPos.size();
		m_vfVertices = new vector<float>[vPos.size()][LAST];
	}

	assert(m_nVertices == vPos.size() && "Incorect Number of Vertices");

	for (unsigned int i = 0; i < m_nVertices; ++i)
	{
		m_vfVertices[i][iAttribute].reserve(3);
		m_vfVertices[i][iAttribute].push_back(vPos[i].r);
		m_vfVertices[i][iAttribute].push_back(vPos[i].g);
		m_vfVertices[i][iAttribute].push_back(vPos[i].b);
	}

	m_pDimensions[iAttribute] = 3;
}

void VertexData::add(Attribute iAttribute, const vector<ColorA>& vPos)
{
	assert(iAttribute < LAST && iAttribute >= 0 && "Invalid Attribute.");

	if (m_vfVertices == nullptr)
	{
		m_nVertices = vPos.size();
		m_vfVertices = new vector<float>[vPos.size()][LAST];
	}

	assert(m_nVertices == vPos.size() && "Incorect Number of Vertices");

	for (unsigned int i = 0; i < m_nVertices; ++i)
	{
		m_vfVertices[i][iAttribute].reserve(4);
		m_vfVertices[i][iAttribute].push_back(vPos[i].r);
		m_vfVertices[i][iAttribute].push_back(vPos[i].g);
		m_vfVertices[i][iAttribute].push_back(vPos[i].b);
		m_vfVertices[i][iAttribute].push_back(vPos[i].a);
	}

	m_pDimensions[iAttribute] = 4;
}

void VertexData::remove(Attribute iAttribute)
{
	for (unsigned int i = 0; i < m_nVertices; ++i)
	{
		m_vfVertices[i][iAttribute].clear();
	}

	m_pDimensions[iAttribute] = 0;

	for (int e : m_pDimensions)
		if (e != 0)
			return;

	delete[] m_vfVertices;
	m_vfVertices = nullptr;
}

void VertexData::clear()
{
	delete [] m_vfVertices;
	m_vfVertices = nullptr;
	for (auto & e : m_pDimensions)
		e = 0;
	m_vuiIndices = {};
}


int VertexData::getStride() const
{
	int nSum = 0;
	for (int e : m_pDimensions)
	{
		nSum += e;
	}
	return nSum;
}

int VertexData::getOffset(Attribute iAttribute) const
{
	int iOffset = 0;
	for (int i = 0; i < iAttribute; ++i)
	{
		iOffset += m_pDimensions[i];
	}
	return iOffset;
}

vector<float> * VertexData::synthesize() const
{
	vector<float> * vfVertices = new vector<float>();
	this->synthesize(vfVertices);
	return vfVertices;
}

void VertexData::synthesize(vector<float> * vfVertices) const
{
	// reserve about as much initial capacity as needed
	vfVertices->clear();
	vfVertices->reserve(m_nVertices * LAST * 3);
	for (unsigned int iPoint = 0; iPoint < m_nVertices; ++iPoint)
	{
		for (const auto & eAttribute : m_vfVertices[iPoint])
		{
			for (float e : eAttribute)
			{
				vfVertices->push_back(e);
			}
		}
	}
	vfVertices->shrink_to_fit();
}

void VertexData::test()
{

}

