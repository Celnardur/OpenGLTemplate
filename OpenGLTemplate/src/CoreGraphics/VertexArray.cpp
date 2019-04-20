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
