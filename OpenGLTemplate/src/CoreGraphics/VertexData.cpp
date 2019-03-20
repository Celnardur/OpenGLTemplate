#include "VertexData.h"
#include <cassert>

VertexData::VertexData()
{
	m_pDimensions = new int[5];
}

VertexData::VertexData(Attribute iAttribute, const vector<Point3D>& vPos)
{
	m_pDimensions = new int[5];
}


VertexData::~VertexData()
{

}

VertexData::VertexData(const VertexData& toCopy)
{

}

VertexData::VertexData(VertexData&& toMove) noexcept
{

}

VertexData& VertexData::operator=(VertexData toSwap)
{

}

void swap(VertexData& first, VertexData& second) noexcept
{

}


void VertexData::add(Attribute iAttribute, const vector<Point2D>& vPos)
{
	m_pDimensions[iAttribute] = 2;

}

void VertexData::add(Attribute iAttribute, const vector<Point3D>& vPos)
{
	assert(iAttribute < 5 && iAttribute >= 0 && "Invalid Attribute.");
	if (m_pDimensions[iAttribute] != 0)
	{
		this->remove(iAttribute);
	}
	m_pDimensions[iAttribute] = 3;


}

void VertexData::add(Attribute iAttribute, const vector<Color>& vPos)
{

}

int VertexData::getStride()
{
	return 0;
}

int* VertexData::getDimensions()
{
	return nullptr;
}

