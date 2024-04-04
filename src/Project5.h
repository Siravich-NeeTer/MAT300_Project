#pragma once

#include "IProject.h"

class Project5 : public IProject
{
public:
	Project5(GraphTable& graphTable);
	~Project5();

	void Update(Window& window, Camera& camera, const float& dt) override;
	void UpdateUI() override;

private:
	// Our Main Graph
	Graph* m_MainGraph;
	int N;

	void InitGraph();
	void PlotGraph();

	int* UpdateDegree();

};