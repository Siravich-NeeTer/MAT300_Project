#pragma once

#include "IProject.h"

class Project2 : public IProject
{
	public:
		Project2(GraphTable& graphTable);
		~Project2();

		void Update(Window& window, Camera& camera, const float& dt) override;
		void UpdateUI() override;

	private:
		// Our Main Graph
		Graph* m_MainGraph;
		Graph* m_DraftLineGraph;
		DragPoint* m_ControlPoint_t;
		float m_Current_t = 1.0f;

		bool isHidePolyLine = false;

		void InitGraph();
		void PlotGraph();

		int* UpdateDegree();

};