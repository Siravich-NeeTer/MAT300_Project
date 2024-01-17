#include "VertexArrayObject.h"

// Constructor -> Generate VertexArrayObject
VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &m_ID);
}

void VertexArrayObject::Bind()
{
	glBindVertexArray(m_ID);
}
void VertexArrayObject::UnBind()
{
	glBindVertexArray(0);
}

// Delete Vertex Array via ID
void VertexArrayObject::Delete()
{
	glDeleteVertexArrays(1, &m_ID);
}
// Bind VBO and Specific Data(AttribPointer) of that VBO
void VertexArrayObject::Attribute(VertexBufferObject& vbo, GLuint index, GLint size, GLenum varType, GLsizei stride, GLsizei offset)
{
	vbo.Bind();
	glVertexAttribPointer(index, size, varType, GL_FALSE, stride, (const void*)offset);
	glEnableVertexAttribArray(index);
	vbo.UnBind();
}

// Get VertexArrayObject ID
GLuint VertexArrayObject::GetID() const
{
	return m_ID;
}