#pragma once

#include <glad/glad.h>
#include "VertexBufferObject.h"

class VertexArrayObject
{
	public:
		VertexArrayObject();
		
		void Bind();
		void UnBind();
		
		void Delete();
		void Attribute(VertexBufferObject& vbo, GLuint index, GLint size, GLenum varType, GLsizei stride, GLsizei offset);
		
		GLuint GetID() const;

	private:
		GLuint m_ID;
};