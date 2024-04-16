#include "Project7.h"

Project7::Project7(GraphTable& graphTable)
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
Project7::~Project7()
{
	m_RefGraphTable->RemoveGraph(m_MainGraph);
	m_RefDragPointController->ClearAllDragPoint();
}

void Project7::Update(Window& window, Camera& camera, const float& dt)
{
	pointSelectIndex = -1;
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
		if (m_ControlPoints[idx]->isSelect)
		{
			pointSelectIndex = idx;
		}
	}

	// Update currentMousePosition
	m_CurrentMousePosition = ConvertMouseToWorldSpace(window, camera, ConvertMouseToNDC(window, { Input::mouseX, Input::mouseY }));
	if (Input::IsKeyBeginPressed(GLFW_MOUSE_BUTTON_LEFT) && degree < 20)
	{
		// Create New Control Points
		m_KnotSequence.push_back(m_KnotSequence.back() + 1);
		//m_KnotSequence.push_back(m_KnotSequence.back() + 1);
		m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

		// Update Control Points' position to cursor position
		m_ControlPoints.back()->position = m_CurrentMousePosition;

		PlotGraph();
	}
	else if (Input::IsKeyBeginPressed(GLFW_MOUSE_BUTTON_MIDDLE) && pointSelectIndex != -1)
	{
		m_RefDragPointController->RemoveDragPoint(m_ControlPoints[pointSelectIndex]);
		m_ControlPoints.erase(m_ControlPoints.begin() + pointSelectIndex);

		m_KnotSequence.pop_back();

		PlotGraph();
	}

	UpdateUI();
}
void Project7::UpdateUI()
{
	ImGui::Begin("Project_7");

	// -------------------- Print Mouse Position --------------------
	std::stringstream mousePos, curPos, tPos;
	mousePos << std::fixed << std::setprecision(3) << m_CurrentMousePosition.x << ',' << m_CurrentMousePosition.y;
	ImGui::Text(std::string("Mouse Position: (" + mousePos.str() + ")").c_str());

	curPos << std::fixed << std::setprecision(3) << m_ControlPoint_t->position.x << ',' << m_ControlPoint_t->position.y;
	tPos << std::fixed << "(t = " << std::setprecision(3) << m_Current_t << ")";
	ImGui::Text(std::string("Current Position " + tPos.str() + ": (" + curPos.str() + ")").c_str());
	// --------------------------------------------------------------

	std::stringstream sKnotSequence;
	sKnotSequence << std::fixed << std::setprecision(1);
	sKnotSequence << "Knot Sequence : [";
	for (size_t i = 0; i < m_KnotSequence.size(); i++)
	{
		sKnotSequence << m_KnotSequence[i];
		if (i < m_KnotSequence.size() - 1)
			sKnotSequence << ", ";
		if (i > 0 && i % 7 == 0)
			sKnotSequence << "\n";
	}
	sKnotSequence << "]";
	ImGui::Text(sKnotSequence.str().c_str());

	if (ImGui::CollapsingHeader("Knot Sequence"))
	{
		for (int i = 0; i < m_KnotSequence.size(); i++)
		{
			std::string text = "t" + std::to_string(i);

			float v_min = (i == 0 ? m_KnotSequence[i + 1] - 1.0f : m_KnotSequence[i - 1]);
			float v_max = (i == m_KnotSequence.size() - 1 ? m_KnotSequence[i - 1] + 1.0f : m_KnotSequence[i + 1]);

			if (ImGui::SliderFloat(text.c_str(), &m_KnotSequence[i], v_min, v_max))
			{
				PlotGraph();
			}
		}
	}

	ImGui::Text(std::string("Degree : " + std::to_string(degree)).c_str());
	ImGui::SameLine();
	if (ImGui::SmallButton("-")) { if (degree > 1) { degree--; m_KnotSequence.pop_back(); } PlotGraph(); }
	ImGui::SameLine();
	if (ImGui::SmallButton("+")) { if (degree < 20) { degree++; m_KnotSequence.push_back(m_KnotSequence.back() + 1); } PlotGraph(); }


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
	if (ImGui::Checkbox("Hide Shell", &isHideShell))
	{
		PlotGraph();
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
		for (int i = 0; i < m_ControlPoints.size(); i++)
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

void Project7::InitGraph()
{
	// Add new Graph
	m_DraftLineGraph = m_RefGraphTable->AddGraph("Draft Line", { 1.0f, 0.0f, 0.0f });
	m_DraftLineGraph->SetThickness(2.0f);
	m_MainGraph = m_RefGraphTable->AddGraph("Interpolating Curve", { 0.0f, 1.0f, 0.0f });

	// Initialize Degree = 3
	degree = 3;
	isHidePoint = false;

	for (int i = 0; i < degree + 1; i++)
	{
		m_KnotSequence.push_back(i);
	}

	m_RefDragPointController->SetDragType(DragPoint::DragType::NONE);
	m_ControlPoint_t = m_RefDragPointController->AddDragPoint(glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
	m_ControlPoint_t->isActive = false;
	m_RefDragPointController->SetDragType(DragPoint::DragType::DRAG_XY);
}
void Project7::PlotGraph()
{
	// Clear existing Graph first
	m_DraftLineGraph->Clear();
	m_MainGraph->Clear();
	// Clear Shell
	for (int i = 0; i < m_Shells.size(); i++)
	{
		m_Shells[i]->Clear();
		m_Shells[i]->SetActive(false);
	}

	if (m_ControlPoints.size() < 2)
	{
		m_ControlPoint_t->isActive = false;
		return;
	}

	std::vector<glm::vec2> positionList(m_ControlPoints.size());
	// Update Coefficients (aka. Base) to std::vector
	for (int i = 0; i < m_ControlPoints.size(); i++)
	{
		positionList[i] = m_ControlPoints[i]->position;
		m_DraftLineGraph->Plot(m_ControlPoints[i]->position);
	}

	if (degree >= m_ControlPoints.size())
	{
		m_ControlPoint_t->isActive = false;
		return;
	}

	m_ControlPoint_t->isActive = true;

	glm::vec2 pos(0.0f);
	float from = m_KnotSequence[degree];
	float to = Lerp(m_KnotSequence[degree], m_KnotSequence[m_KnotSequence.size() - degree - 1], m_Current_t);
	for (float t = from; t <= to; t += 0.001f)
	{
		pos = DeBoor_BSpline(positionList, m_KnotSequence, degree, t);
		m_MainGraph->Plot(pos);
	}
	m_ControlPoint_t->position = pos;

	if (isHideShell)
		return;

	float epsilon = 0.001f;
	std::vector<std::vector<glm::vec2>> shellsList;
	DeBoor_BSpline(positionList, m_KnotSequence, degree, Clamp(to, m_KnotSequence[degree] + epsilon, m_KnotSequence[m_KnotSequence.size() - degree - 1] - epsilon), shellsList);
	// If ShellList > currentShell Graph
	while (shellsList.size() > m_Shells.size())
	{
		m_Shells.push_back(m_RefGraphTable->AddGraph("Shell " + std::to_string(m_Shells.size() + 1), { 1.0f, 0.0f, 1.0f }));
	}
	// If ShellList < currentShell Graph
	for (int i = m_Shells.size(); i > shellsList.size(); i--)
	{
		m_Shells[i - 1]->SetActive(false);
	}
	for (int i = 0; i < shellsList.size(); i++)
	{
		m_Shells[i]->Clear();
		m_Shells[i]->SetActive(true);
		for (int j = 0; j < shellsList[i].size(); j++)
		{
			m_Shells[i]->Plot(shellsList[i][j]);
		}
	}
}