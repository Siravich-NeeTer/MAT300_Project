#pragma once

#include "CustomMath.h"
#include "GraphTable.h"
#include "DragPointController.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iomanip>

enum GRAPH_METHOD
{
	BB,			// Berstein Polynomials
	NLI,		// Nested Linear Interpolation
	MID_POINT,
	NONE
};

// Interface class for Project
class IProject
{
	public:
		IProject(GraphTable& graphTable)
		{
			// Get Pointer of GraphTable & DragPointController
			m_RefGraphTable = &graphTable;
			m_RefDragPointController = DragPointController::GetInstance();
		}
		virtual ~IProject()
		{
			m_RefGraphTable->Reset();
		}

		virtual void Update(Window& window, Camera& camera, const float& dt) = 0;
		virtual void UpdateUI() = 0;

	protected:
		// Reference to GraphTable & DragPoint(Moveable Point)Controller
		GraphTable* m_RefGraphTable;
		DragPointController* m_RefDragPointController;

		GRAPH_METHOD m_CurrentGraphMethod;
		glm::vec3 m_CurrentMousePosition;

		// Adjustable components
		int degree;
		bool isHidePoint;
		std::vector<DragPoint*> m_ControlPoints;

		virtual std::string GetGraphingMethod() const 
		{
			switch (m_CurrentGraphMethod)
			{
				case GRAPH_METHOD::BB: return "BB";
				case GRAPH_METHOD::NLI: return "NLI";
				case GRAPH_METHOD::MID_POINT: return "MID POINT";
			}
		}
};