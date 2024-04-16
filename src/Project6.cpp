#include "Project6.h"

Project6::Project6(GraphTable& graphTable)
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
Project6::~Project6()
{
	m_RefGraphTable->RemoveGraph(m_MainGraph);
	m_RefDragPointController->ClearAllDragPoint();
}

void Project6::Update(Window& window, Camera& camera, const float& dt)
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
		m_OrgPositionList.back() = m_ControlPoints.back()->position;
		PlotGraph();
	}

	UpdateUI();
}
void Project6::UpdateUI()
{
	ImGui::Begin("Project_6");

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
	}
	sKnotSequence << "]";
	ImGui::Text(sKnotSequence.str().c_str());

	if (ImGui::Button("Toggle Knot Sequence to Default"))
	{
		isKnotSequenceDefault = !isKnotSequenceDefault;
		PlotGraph();
	}

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
			if (ImGui::SliderFloat2(std::string((isKnotSequenceDefault ? "Q" : "P") + std::to_string(i)).c_str(), &m_ControlPoints[i]->position[0], -10.0f, 10.0f))
			{
				PlotGraph();
			}
		}
	}
	// --------------------------------------------------------------

	ImGui::End();
}

void Project6::InitGraph()
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
	m_OrgPositionList.push_back(m_ControlPoints.back()->position);
	m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_OrgPositionList.push_back(m_ControlPoints.back()->position);

	m_RefDragPointController->SetDragType(DragPoint::DragType::NONE);
	m_ControlPoint_t = m_RefDragPointController->AddDragPoint(m_ControlPoints[0]->position, glm::vec3(1.0f, 0.0f, 1.0f));
	m_RefDragPointController->SetDragType(DragPoint::DragType::DRAG_XY);
}
void Project6::PlotGraph()
{
	// Update Knot Sequence
	m_KnotSequence.clear();
	if (isKnotSequenceDefault)
	{
		for (int i = 0; i < degree; i++)
			m_KnotSequence.push_back(-(degree - i) / 10.0f);
		m_KnotSequence.push_back(0.0f);
		m_KnotSequence.push_back(1.0f);
		for (int i = 1; i <= degree; i++)
			m_KnotSequence.push_back(1 + (i / 10.0f));
	}
	else
	{
		// Naive Knot
		for (int i = 0; i < degree + 1; i++)
			m_KnotSequence.push_back(0);
		for (int i = 0; i < degree + 1; i++)
			m_KnotSequence.push_back(1);
	}
	
	// Clear existing Graph first
	m_DraftLineGraph->Clear();
	m_MainGraph->Clear();

	if (isPointMove)
		m_OrgPositionList[pointMoveIndex] = m_CurrentMousePosition;

	std::vector<glm::vec2> positionList(degree + 1);
	// Update Coefficients (aka. Base) to std::vector
	for (int i = 0; i <= degree; i++)
	{
		positionList[i] = m_OrgPositionList[i];
	}

	// Plot Graph
	std::vector<glm::vec2> controlPoints_Q;
	for (int i = 1; i <= degree + 1; i++)
	{
		glm::vec2 result = PolarForm(positionList, m_KnotSequence, i, i + degree - 1);
		controlPoints_Q.push_back(result);
	}

	for (int i = 0; i <= degree; i++)
	{
		m_ControlPoints[i]->position = glm::vec3(controlPoints_Q[i], 0.0f);
		m_DraftLineGraph->Plot(m_ControlPoints[i]->position);
	}

	glm::vec2 pos;
	for (float t = 0.0f; t <= m_Current_t; t += 0.001f)
	{
		pos = NestedLinearInterpolation_DeBoor(controlPoints_Q, m_KnotSequence, t);
		m_MainGraph->Plot(pos);
	}
	m_ControlPoint_t->position = glm::vec3(pos, 0.0f);
}

int* Project6::UpdateDegree()
{
	while (degree > m_ControlPoints.size() - 1)
	{
		m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(0.0f, 0.0f), m_ControlPoints[0]->color));
		m_OrgPositionList.push_back(m_ControlPoints.back()->position);
	}
	while (degree < m_ControlPoints.size() - 1)
	{
		m_RefDragPointController->RemoveDragPoint(m_ControlPoints.back());
		m_ControlPoints.pop_back();
		m_OrgPositionList.pop_back();
	}

	return &degree;
}