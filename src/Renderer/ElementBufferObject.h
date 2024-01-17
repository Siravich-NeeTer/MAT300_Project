#pragma once

#include <glad/glad.h>

class ElementBufferObject
{
	public:
		ElementBufferObject();

		void Bind();
		void UnBind();

		void Delete();
		void BufferData(GLsizei dataSize, const void* data, GLboolean dynamic);

		GLuint GetID() const;
	private:
		GLuint m_ID;
};