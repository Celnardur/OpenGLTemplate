#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "GraphicsIncludes.h"
#include "VertexData.h"

class VertexArray
{
public:
	VertexArray();

	template <class Vertex>
	void create(const VertexData<Vertex> & vertexData);

	void bind() const;
	void render() const;
	void unbind() const;
	void destroy();


private:
	unsigned int m_uidVertexArrayObject;
	// save these two kept just so that their memory can be freed
	unsigned int m_uidVertexBufferObject;
	unsigned int m_uidElementBufferObject;

	std::vector<int> m_vcAttributes;
	bool m_bHasIndices;
	size_t m_nDrawPoints;
};

template <class Vertex>
void VertexArray::create(const VertexData<Vertex>& vertexData)
{
	m_bHasIndices = !vertexData.vuiIndices.empty();
	if (m_bHasIndices)
		m_nDrawPoints = vertexData.vuiIndices.size();
	else
		m_nDrawPoints = vertexData.vertices.size();



	int iStride = vertexData.getStride();

	glGenVertexArrays(1, &m_uidVertexArrayObject);
	glGenBuffers(1, &m_uidVertexBufferObject);

	glBindVertexArray(m_uidVertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_uidVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, iStride * vertexData.vertices.size(),
		&(vertexData.vertices[0]), GL_STATIC_DRAW);

	if (m_bHasIndices)
	{
		glGenBuffers(1, &m_uidElementBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uidElementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexData.vuiIndices.size(),
			&(vertexData.vuiIndices[0]), GL_STATIC_DRAW);
	}

	int iOffset = 0;
	for (int i = 0; i < LAST; ++i)
	{
		if (vertexData.aDimensions[i] > 0)
		{
			glVertexAttribPointer(i, vertexData.aDimensions[i], GL_FLOAT, GL_FALSE,
				iStride, (void*)iOffset);
			iOffset += vertexData.aDimensions[i] * sizeof(float);
			m_vcAttributes.push_back(i);
		}
	}
}

#endif