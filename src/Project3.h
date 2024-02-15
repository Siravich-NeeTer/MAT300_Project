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

		bool isHidePolyLine = false;

		void InitGraph();
		void PlotGraph();

		int* UpdateDegree();

};