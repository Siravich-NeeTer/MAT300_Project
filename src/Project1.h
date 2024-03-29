#pragma once

#include "IProject.h"

class Project1 : public IProject
{
	public:
		Project1(GraphTable& graphTable);
		~Project1();

		void Update(Window& window, Camera& camera, const float& dt) override;
		void UpdateUI() override;
	
	private:
		// Our Main Graph
		Graph* m_MainGraph;

		void InitGraph();
		void PlotGraph();

		int* UpdateDegree();

};