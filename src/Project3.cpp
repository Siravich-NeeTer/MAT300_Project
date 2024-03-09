#include "Project3.h"

Project3::Project3(GraphTable& graphTable)
	: IProject(graphTable)
{ 
	// Init GraphTable
	graphTable.InitTable(GraphTable::TableType::XY_TABLE, { -10.0f, 10.0f }, { -10.0f, 10.0f });
	m_CurrentGraphMethod = GRAPH_METHOD::NONE;

	// Set DragPointController Type
	m_RefDragPointController->SetDragType(DragPoint::DragType::DRAG_XY);

	// Initialize
	InitGraph();
	PlotGraph();
}
Project3::~Project3()
{
	m_RefGraphTable->RemoveGraph(m_MainGraph);
	m_RefDragPointController->ClearAllDragPoint();
}

void Project3::Update(Window& window, Camera& camera, const float& dt)
{
	// If ControlPoints is empty (degree = 0) -> no need to update
	if (m_ControlPoints.empty())
		return;

	isPointMove = false;
	for (int idx = 0; idx < m_ControlPoints.size(); idx++)
	{
		// If one of ControlPoints move -> Update entire graph
		if (m_ControlPoints[idx]->isMove)
		{
			// Plot new one
			isPointMove = true;
			pointMoveIndex = idx;
			PlotGraph();
		}
	}

	// Update currentMousePosition
	m_CurrentMousePosition = ConvertMouseToWorldSpace(window, camera, ConvertMouseToNDC(window, { Input::mouseX, Input::mouseY }));
	if (Input::IsKeyBeginPressed(GLFW_MOUSE_BUTTON_LEFT) && degree < 20)
	{
		degree++;
		UpdateDegree();
		m_ControlPoints.back()->position = m_CurrentMousePosition;
		PlotGraph();
	}

	UpdateUI();
}
void Project3::UpdateUI()
{
	ImGui::Begin("Project_3");

	// -------------------- Print Mouse Position --------------------
	std::stringstream mousePos, curPos, tPos;
	mousePos << std::fixed << std::setprecision(3) << m_CurrentMousePosition.x << ',' << m_CurrentMousePosition.y;
	ImGui::Text(std::string("Mouse Position: (" + mousePos.str() + ")").c_str());

	curPos << std::fixed << std::setprecision(3) << m_ControlPoint_t->position.x << ',' << m_ControlPoint_t->position.y;
	tPos << std::fixed << "(t = " << std::setprecision(3) << m_Current_t << ")";
	ImGui::Text(std::string("Current Position " + tPos.str() + ": (" + curPos.str() + ")").c_str());
	// --------------------------------------------------------------

	ImGui::Text(std::string("Degree : " + std::to_string(degree)).c_str());
	ImGui::SameLine();
	if (ImGui::SmallButton("-")) { if (degree > 1) { degree--; } UpdateDegree(); PlotGraph(); }
	ImGui::SameLine();
	if (ImGui::SmallButton("+")) { if (degree < 20) { degree++; } UpdateDegree(); PlotGraph(); }


	if (ImGui::SliderFloat("t", &m_Current_t, 0.0f, 1.0f))
	{
		PlotGraph();
	}

	if (ImGui::Checkbox("Hide Point", &isHidePoint))
	{
		for (int idx = 0; idx < m_ControlPoints.size(); idx++)
		{
			m_ControlPoints[idx]->isActive = !isHidePoint;
		}
	}
	if (ImGui::Checkbox("Hide Polyline", &isHidePolyLine))
	{
		m_DraftLineGraph->SetActive(!isHidePolyLine);
	}
	if (ImGui::Checkbox("Optimize", &isOptimize))
	{
		// Optimize
		// - When Point Move -> Update only that index and lower
		// - When Add New Point -> Add one point to table aka. other value still same
	}

	// ---------------------- Graphing Method -----------------------
	if (ImGui::CollapsingHeader("Graph"))
	{
		ImGui::NewLine();

		ImGui::SliderFloat("Graph Thickness", &m_MainGraph->GetThickness(), 1.0f, 8.0f);

		ImGui::NewLine();

		ImGui::ColorPicker3("Graph Color", &m_MainGraph->GetColor()[0]);

	}
	// ----------------------- Control Point ------------------------
	if (ImGui::CollapsingHeader("Control Point"))
	{
		for (int i = 0; i <= degree; i++)
		{
			if (ImGui::SliderFloat2(std::string("P" + std::to_string(i)).c_str(), &m_ControlPoints[i]->position[0], -10.0f, 10.0f))
			{
				PlotGraph();
			}
		}
	}
	// --------------------------------------------------------------

	ImGui::End();
}

void Project3::InitGraph()
{
	// Add new Graph
	m_DraftLineGraph = m_RefGraphTable->AddGraph("Draft Line", { 1.0f, 0.0f, 0.0f });
	m_DraftLineGraph->SetThickness(2.0f);
	m_MainGraph = m_RefGraphTable->AddGraph("Interpolating Curve", { 0.0f, 1.0f, 0.0f });

	// Initialize Degree = 1
	degree = 1;
	isHidePoint = false;

	// Initialize Control Points a0, a1
	m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	m_RefDragPointController->SetDragType(DragPoint::DragType::NONE);
	m_ControlPoint_t = m_RefDragPointController->AddDragPoint(m_ControlPoints[0]->position, glm::vec3(1.0f, 0.0f, 1.0f));
	m_RefDragPointController->SetDragType(DragPoint::DragType::DRAG_XY);
}
void Project3::PlotGraph()
{	// Clear existing Graph first
	m_DraftLineGraph->Clear();
	m_MainGraph->Clear();

	for (int i = 0; i <= degree; i++)
	{
		m_DraftLineGraph->Plot(m_ControlPoints[i]->position);
	}

	while (m_PositionList.size() < m_ControlPoints.size())
	{
		int idx = m_PositionList.size();
		m_PositionList.push_back(m_ControlPoints[idx]->position);
		m_ti.push_back(m_ti.size());
	}
	while (m_PositionList.size() > m_ControlPoints.size())
	{
		auto it = std::find(m_ti.begin(), m_ti.end(), m_PositionList.size() - 1);
		int idx = it - m_ti.begin();
		m_PositionList.erase(m_PositionList.begin() + idx);
		m_ti.erase(it);
	}

	if (isPointMove)
	{
		if (m_ti.back() != pointMoveIndex)
			m_PositionList[pointMoveIndex] = m_ControlPoints[pointMoveIndex]->position;
		else
			m_PositionList.back() = m_ControlPoints[pointMoveIndex]->position;
	}

	// If Table is Empty -> Start Create Table
	if (m_CoeffTable_X.empty() && m_CoeffTable_Y.empty())
	{
		CreateNewtonFormTable(m_PositionList, NewtonFormType::X, m_CoeffTable_X);
		CreateNewtonFormTable(m_PositionList, NewtonFormType::Y, m_CoeffTable_Y);
	}
	else if(isOptimize)
	{
		// Move Interpolate Point
		if (isPointMove)
		{
			if (m_ti.back() != pointMoveIndex)
			{
				std::swap(m_PositionList[pointMoveIndex], m_PositionList.back());
				std::swap(m_ti[pointMoveIndex], m_ti.back());
				UpdateNewtonFormTable(m_PositionList, NewtonFormType::X, m_CoeffTable_X, m_ti, glm::clamp(pointMoveIndex - 1, 0, degree));
				UpdateNewtonFormTable(m_PositionList, NewtonFormType::Y, m_CoeffTable_Y, m_ti, glm::clamp(pointMoveIndex - 1, 0, degree));
			}
			else
			{
				UpdateNewtonFormTable(m_PositionList, NewtonFormType::X, m_CoeffTable_X, m_ti, degree);
				UpdateNewtonFormTable(m_PositionList, NewtonFormType::Y, m_CoeffTable_Y, m_ti, degree);
			}
		}
		// Create New Point
		else
		{
			UpdateNewtonFormTable(m_PositionList, NewtonFormType::X, m_CoeffTable_X, m_ti, degree);
			UpdateNewtonFormTable(m_PositionList, NewtonFormType::Y, m_CoeffTable_Y, m_ti, degree);
		}
	}
	else
	{
		UpdateNewtonFormTable(m_PositionList, NewtonFormType::X, m_CoeffTable_X, m_ti);
		UpdateNewtonFormTable(m_PositionList, NewtonFormType::Y, m_CoeffTable_Y, m_ti);
	}

	for (float t = 0.0f; t <= m_Current_t * degree; t += (degree / 1000.0f))
	{
		double x = SubstituteNewtonForm(t, m_CoeffTable_X, m_ti);
		double y = SubstituteNewtonForm(t, m_CoeffTable_Y, m_ti);

		m_MainGraph->Plot({ x, y });
	}
	m_MainGraph->Plot({ SubstituteNewtonForm(m_Current_t * degree, m_CoeffTable_X, m_ti), SubstituteNewtonForm(m_Current_t * degree, m_CoeffTable_Y, m_ti) });

	m_ControlPoint_t->position = m_MainGraph->GetPointList().back();
}

int* Project3::UpdateDegree()
{
	while (degree > m_ControlPoints.size() - 1)
		m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(0.0f, 0.0f), m_ControlPoints[0]->color));
	while (degree < m_ControlPoints.size() - 1)
	{
		m_RefDragPointController->RemoveDragPoint(m_ControlPoints.back());
		m_ControlPoints.pop_back();
	}

	return &degree;
}