#pragma once

#include "CustomMath.h"
#include "GraphTable.h"
#include "DragPointController.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iomanip>

enum GRAPH_METHOD
{
	BB,		// Berstein Polynomials
	NLI		// Nested Linear Interpolation
};

class Project1
{
	public:
		Project1(GraphTable& graphTable);

		void Update(Window& window, Camera& camera, const float& dt);
		void UpdateUI();
	
	private:
		// Reference to GraphTable & DragPoint(Moveable Point)Controller
		GraphTable* m_GraphTable;
		DragPointController* m_RefDragPointController;

		// Our Main Graph
		Graph* m_MainGraph;

		GRAPH_METHOD m_CurrentGraphMethod;
		glm::vec3 m_CurrentMousePosition;

		// Adjustable components
		int degree;
		bool isHidePoint;
		std::vector<DragPoint*> m_ControlPoints;

		void InitGraph();
		void PlotGraph();

		int* UpdateDegree();
		std::string GetGraphingMethod() const;

};