#pragma once

#include "IProject.h"

#include <Eigen/Dense>

class Project7 : public IProject
{
public:
	Project7(GraphTable& graphTable);
	~Project7();

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

	std::vector<glm::vec2> m_PositionList;
	std::vector<float> m_KnotSequence;

	bool isHidePolyLine = false;
	bool isPointMove = false;
	bool isHideShell = false;
	int pointSelectIndex = -1;
	int pointMoveIndex = -1;

	void InitGraph();
	void PlotGraph();

};