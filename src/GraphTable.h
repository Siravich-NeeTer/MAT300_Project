#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Renderer/VertexArrayObject.h"
#include "Renderer/VertexBufferObject.h"
#include "Renderer/ElementBufferObject.h"
#include "Renderer/Shader.h"
#include "Renderer/TextRenderer.h"

#include "Camera.h"

class Graph
{
	public:
		Graph(const std::string& _name, const glm::vec3& _color)
			: name(_name), color(_color), thickness(4.0f)
		{
			m_Graph_VBO.Bind();
			m_Graph_VBO.BufferData(m_PointsLimit * sizeof(glm::vec2), nullptr, true);
			m_Graph_VBO.UnBind();
		}

		void Plot(const glm::vec2& pos)
		{
			m_Points.push_back(pos);

			if (!m_HavePointsLimit && m_Points.size() >= m_PointsLimit)
			{
				m_PointsLimit *= 2;
				m_Graph_VBO.Bind();
				m_Graph_VBO.BufferData(m_PointsLimit * sizeof(glm::vec2), nullptr, true);
			}
			m_IsBind = false;
		}
		void Render(Shader& shader)
		{
			if (!m_IsBind)
			{
				m_Graph_VAO.Bind();
				m_Graph_VBO.Bind();

				m_Graph_VBO.BufferSubData(0, sizeof(glm::vec2) * m_Points.size(), m_Points.data());
				m_Graph_VAO.Attribute(m_Graph_VBO, 0, 2, GL_FLOAT, sizeof(glm::vec2), 0);
				m_IsBind = true;
			}

			glLineWidth(thickness);
			shader.SetVec3("u_Color", color);

			m_Graph_VAO.Bind();
			glDrawArrays(GL_LINE_STRIP, 0, m_Points.size());
		}
		void Clear()
		{
			m_Points.clear();
		}

		void SetName(const std::string& name)
		{
			this->name = name;
		}
		void SetColor(const glm::vec3& color)
		{
			this->color = color;
		}
		void SetThickness(const float& thickness)
		{
			this->thickness = thickness;
		}

		std::string &GetName() { return name; }
		glm::vec3 &GetColor() { return color; }
		float& GetThickness() { return thickness; }

		bool operator==(const Graph& lhs)
		{
			return name == lhs.name;
		}

	private:
		std::string name;
		glm::vec3 color;
		float thickness;

		VertexArrayObject m_Graph_VAO;
		VertexBufferObject m_Graph_VBO;

		std::vector<glm::vec2> m_Points;

		int m_PointsLimit = 100;
		bool m_HavePointsLimit = false;
		bool m_IsBind = false;
};

class GraphTable
{
	public:
		enum TableType
		{
			XY_TABLE, YT_TABLE
		};

		GraphTable();
		~GraphTable();

		void InitTable(TableType tableType, const glm::vec2& verticalSize = { -1.0f, 1.0f }, const glm::vec2& horizontalSize = { -1.0f, 1.0f });
		void Render(const Window& window, const Camera& camera);

		Graph* AddGraph(const std::string& name = "", const glm::vec3& color = { 1.0f, 0.0f, 0.0f });
		bool RemoveGraph(Graph* graph);

		void Reset();

		float GetTableScale() const { return tScale; }

	private:
		// Table Component
		// - Buffer
		// -- Table
		VertexArrayObject m_Table_VAO;
		VertexBufferObject m_Table_VBO;
		std::vector<glm::vec2> m_TableVertices;
		// -- Graph
		std::vector<Graph*> m_Graphs;
		// - Shader
		Shader m_TableShader;

		const float tScale = 6.0f;
};