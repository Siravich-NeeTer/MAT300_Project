#pragma once

#include "IProject.h"

#include <Eigen/Dense>

class Project4 : public IProject
{
public:
	Project4(GraphTable& graphTable);
	~Project4();

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

	Eigen::MatrixXf m_CoefficientMatrix;

	bool isHidePolyLine = false;
	bool isPointMove = false;
	int pointMoveIndex = 0;

	void InitGraph();
	void PlotGraph();

	int* UpdateDegree();

};