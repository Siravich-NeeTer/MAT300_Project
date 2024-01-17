#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

class Mesh
{
	private:
		VertexArrayObject m_VAO;
		VertexBufferObject m_VBO;
		ElementBufferObject m_EBO;

		void SetupMesh();
		void SetupNormalMesh();

	public:
		std::vector<glm::vec3> vertices;
		std::vector<GLuint> indices;

		Mesh();
		Mesh(const std::vector<glm::vec3>& vertices);
		Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices);
		~Mesh();

		void SetVertices(const std::vector<glm::vec3>& vertices, bool isNormal = false);
		void SetMesh(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices, bool isNormal = false);

		void Draw(GLenum drawMode);
};