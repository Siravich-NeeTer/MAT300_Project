#pragma once

#include "Renderer/VertexArrayObject.h"
#include "Renderer/VertexBufferObject.h"
#include "Renderer/ElementBufferObject.h"
#include "Renderer/Shader.h"

#include <glm/gtx/string_cast.hpp>

#include "Camera.h"

#include <iostream>
#include <vector>

struct DragPoint
{
	glm::vec2 position;
	glm::vec3 color;
	bool isMove;
	bool isActive;

	DragPoint(const glm::vec2& _position, const glm::vec3& _color)
		: position(_position), color(_color), isMove(false), isActive(true)
	{
	}
};

class DragPointController
{
	public:
		enum DragType
		{
			DRAG_X, DRAG_Y, DRAG_XY
		};

		DragPointController()
			: m_Shader("DragPointShader"),
				m_CurrentDragType(DragType::DRAG_XY)
		{
			// Create Shader
			m_Shader.AttachShader(BaseShader("Shader/DragPoint.vert"));
			m_Shader.AttachShader(BaseShader("Shader/DragPoint.frag"));
			m_Shader.LinkProgram();

			glm::vec2 org = { 0.0f, 0.0f };

			m_VAO.Bind();
			m_VBO.Bind();

			m_VBO.BufferData(sizeof(glm::vec2), &org, false);
			m_VAO.Attribute(m_VBO, 0, 2, GL_FLOAT, sizeof(glm::vec2), 0);

			m_VBO.UnBind();
			m_VAO.UnBind();
		}
		
		static DragPointController* GetInstance()
		{
			if (m_Instance == nullptr)
				m_Instance = new DragPointController();

			return m_Instance;
		}
		void Clean()
		{
			for (int idx = m_DragPoints.size() - 1; idx >= 0; idx--)
			{
				delete m_DragPoints[idx];
			}

			if (m_Instance != nullptr)
				delete m_Instance;
		}

		DragPoint* AddDragPoint(glm::vec2 pos, glm::vec3 color)
		{
			m_DragPoints.push_back(new DragPoint(pos, color));

			return m_DragPoints.back();
		}
		void RemoveDragPoint(DragPoint* dragPoint)
		{
			auto it = std::find(m_DragPoints.begin(), m_DragPoints.end(), dragPoint);
			if (it == m_DragPoints.end())
				return;

			m_DragPoints.erase(it);
		}
		void ClearAllDragPoint()
		{
			for (int idx = m_DragPoints.size() - 1; idx >= 0; idx--)
			{
				delete m_DragPoints[idx];
			}
			m_DragPoints.clear();
		}

		void Update(const Window& window, const Camera& camera, const float& dt)
		{
			// DEBUG [print Mouse at World-Coordinate]
			//std::cout << mouseWorld.x << " , " << mouseWorld.y << " , " << mouseWorld.z << "\n";

			glm::vec3 mouseWorld = ConvertMouseToWorldSpace(window, camera, ConvertMouseToNDC(window, glm::vec2(Input::mouseX, Input::mouseY)));
			if (m_CurrentSelectedDragPoint && Input::IsKeyPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				m_CurrentSelectedDragPoint->isMove = true;
				switch (m_CurrentDragType)
				{
					case DragType::DRAG_X: 
						m_CurrentSelectedDragPoint->position.x = mouseWorld.x;
						break;
					case DragType::DRAG_Y:
						m_CurrentSelectedDragPoint->position.y = mouseWorld.y;
						break;
					case DragType::DRAG_XY:
						m_CurrentSelectedDragPoint->position.x = mouseWorld.x;
						m_CurrentSelectedDragPoint->position.y = mouseWorld.y;
						break;
				}
				return;
			}

			m_CurrentSelectedDragPoint = nullptr;
			for (int i = 0; i < m_DragPoints.size(); i++)
			{
				if (!m_DragPoints[i]->isActive)
					continue;

				if (glm::distance(glm::vec2(mouseWorld), m_DragPoints[i]->position) < 0.03f * (m_PointSize / 100.0f))
				{
					m_DragPoints[i]->color = { 0.0f, 1.0f, 1.0f };
					m_CurrentSelectedDragPoint = m_DragPoints[i];
				}
				else
				{
					m_DragPoints[i]->color = { 0.0f, 0.0f, 1.0f };
				}
				m_DragPoints[i]->isMove = false;
			}

		}
		void Render(const Window& window, const Camera& camera)
		{
			m_Shader.Activate();
			m_Shader.SetMat4("u_View", camera.GetViewMatrix());
			m_Shader.SetMat4("u_Projection", camera.GetPerspective(window));
			m_Shader.SetFloat("u_CameraZPos", camera.GetPosition().z);
			m_Shader.SetFloat("u_PointSize", m_PointSize);

			for (int i = 0; i < m_DragPoints.size(); i++)
			{
				if (!m_DragPoints[i]->isActive)
					continue;

				m_Shader.SetMat4("u_Model", glm::translate(glm::mat4(1.0f), glm::vec3(m_DragPoints[i]->position.x, m_DragPoints[i]->position.y, 0.0f)));
				m_Shader.SetVec3("u_Color", m_DragPoints[i]->color);

				m_VAO.Bind();
				glDrawArrays(GL_POINTS, 0, 1);
			}
		}

		void SetDragType(DragType dragType)
		{
			m_CurrentDragType = dragType;
		}
		DragType GetDragType() const { return m_CurrentDragType; }

	private:
		static DragPointController* m_Instance;

		VertexArrayObject m_VAO;
		VertexBufferObject m_VBO;

		Shader m_Shader;

		const float m_PointSize = 75.0f;
		std::vector<DragPoint*> m_DragPoints;
		DragPoint* m_CurrentSelectedDragPoint;
		DragType m_CurrentDragType;
};