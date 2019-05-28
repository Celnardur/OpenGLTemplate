#include "VertexArray.h"
#include "glad/glad.h"

VertexArray::VertexArray()
{
	m_uidVertexArrayObject = 0;
	m_uidVertexBufferObject = 0;
	m_uidElementBufferObject = 0;

	m_bHasIndices = false;
	m_nDrawPoints = 0;
	m_vcAttributes = {};
}

void VertexArray::bind() const
{
	glBindVertexArray(m_uidVertexArrayObject);
	for (auto element : m_vcAttributes) { glEnableVertexAttribArray(element); }
}

void VertexArray::render() const
{
	if (m_bHasIndices)
		glDrawElements(GL_TRIANGLES, m_nDrawPoints, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_nDrawPoints);
}

void VertexArray::unbind() const
{
	for (auto element : m_vcAttributes) { glDisableVertexAttribArray(element); }
}

void VertexArray::destroy()
{
	glDeleteVertexArrays(1, &m_uidVertexArrayObject);
	glDeleteBuffers(1, &m_uidVertexBufferObject);
	glDeleteBuffers(1, &m_uidElementBufferObject);
}

unsigned int VertexArray::bufferData(uint64_t ui64Hash, int iBufferType, size_t szData, void* pData)
{
	static BufferMap bmVBO;
	static BufferMap bmEBO;

	assert(iBufferType == GL_ARRAY_BUFFER || iBufferType == GL_ELEMENT_ARRAY_BUFFER);

	BufferMap * pbmUsed;
	if (iBufferType == GL_ARRAY_BUFFER)
		pbmUsed = &bmVBO;
	else
		pbmUsed = &bmEBO;

	// Store ids in hash table to avoid resending data to GPU
	// Hash map where key is the hash of the Vertices and data is ID
	unsigned int uidBuffer = (*pbmUsed)[ui64Hash];
	if (uidBuffer)
	{
		std::cout << "Using ID: " << uidBuffer << std::endl;
		glBindBuffer(iBufferType, uidBuffer);
	}
	else
	{
		glGenBuffers(1, &uidBuffer);
		glBindBuffer(iBufferType, uidBuffer);
		glBufferData(iBufferType, szData, pData, GL_STATIC_DRAW);
		pbmUsed->insert(ui64Hash, uidBuffer);
	}

	return uidBuffer;
}

