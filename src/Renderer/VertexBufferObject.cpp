#include "VertexBufferObject.h"

// Constructor -> Generate VertexBufferObject
VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &m_ID);
}

void VertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
void VertexBufferObject::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Delete VertexBufferObject via ID
void VertexBufferObject::Delete()
{
	glDeleteBuffers(1, &m_ID);
}
// Bind Buffer and Specific that Buffer
void VertexBufferObject::BufferData(size_t dataSize, const void* data, bool dynamic)
{
	this->Bind();
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, (dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

void VertexBufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data)
{
	this->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

// Get VertexBufferObject ID
GLuint VertexBufferObject::GetID() const
{
	return m_ID;
}