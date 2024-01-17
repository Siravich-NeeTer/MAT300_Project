#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject()
{
	glGenBuffers(1, &m_ID);
}

void ElementBufferObject::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}
void ElementBufferObject::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::Delete()
{
	glDeleteBuffers(1, &m_ID);
}
void ElementBufferObject::BufferData(GLsizei dataSize, const void* data, GLboolean dynamic)
{
	this->Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, (dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

GLuint ElementBufferObject::GetID() const
{
	return m_ID;
}