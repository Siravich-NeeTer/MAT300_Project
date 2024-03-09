#pragma once

#include "IProject.h"

class Project3 : public IProject
{
	public:
		Project3(GraphTable& graphTable);
		~Project3();

		void Update(Window& window, Camera& camera, const float& dt) override;
		void UpdateUI() override;

	private:
		// Our Main Graph
		Graph* m_MainGraph;
		Graph* m_DraftLineGraph;
		std::vector<Graph*> m_Shells;
		DragPoint* m_ControlPoint_t;
		float m_Current_t = 1.0f;
		int m_Current_k = 1;
		std::vector<std::vector<double>> m_CoeffTable_X;
		std::vector<std::vector<double>> m_CoeffTable_Y;
		
		std::vector<glm::vec2> m_PositionList;
		std::vector<int> m_ti;

		bool isHidePolyLine = false;
		bool isOptimize = false;
		bool isPointMove = false;
		int pointMoveIndex = 0;

		void InitGraph();
		void PlotGraph();

		int* UpdateDegree();

};