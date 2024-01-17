#pragma once

#include <glad/glad.h>

class VertexBufferObject
{
	public:
		VertexBufferObject();
		
		void Bind();
		void UnBind();
		
		void Delete();
		void BufferData(size_t dataSize, const void* data, bool dynamic);
		void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
		
		GLuint GetID() const;

	private:
		GLuint m_ID;
};