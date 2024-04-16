#include "Project8.h"

Project8::Project8(GraphTable& graphTable)
	: IProject(graphTable)
{
	// Init GraphTable
	graphTable.InitTable(GraphTable::TableType::XY_TABLE, { -10.0f, 10.0f }, { -10.0f, 10.0f });
	m_CurrentGraphMethod = GRAPH_METHOD::BB;

	// Set DragPointController Type
	m_RefDragPointController->SetDragType(DragPoint::DragType::DRAG_XY);

	// Initialize
	InitGraph();
	PlotGraph();
}
Project8::~Project8()
{
	m_RefGraphTable->RemoveGraph(m_MainGraph);
	m_RefDragPointController->ClearAllDragPoint();
}

void Project8::Update(Window& window, Camera& camera, const float& dt)
{
	// If ControlPoints is empty (degree = 0) -> no need to update
	if (m_ControlPoints.empty())
		return;

	for (int idx = 0; idx < m_ControlPoints.size(); idx++)
	{
		// If one of ControlPoints move -> Update entire graph
		if (m_ControlPoints[idx]->isMove)
		{
			// Plot new one
			PlotGraph();
		}
	}

	// Update currentMousePosition
	m_CurrentMousePosition = ConvertMouseToWorldSpace(window, camera, ConvertMouseToNDC(window, { Input::mouseX, Input::mouseY }));
	if (Input::IsKeyBeginPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		degree++;
		UpdateDegree();
		m_ControlPoints.back()->position = m_CurrentMousePosition;
		PlotGraph();
	}

	UpdateUI();
}
void Project8::UpdateUI()
{
	ImGui::Begin("Project_2");

	// -------------------- Print Mouse Position --------------------
	std::stringstream mousePos, curPos, tPos;
	mousePos << std::fixed << std::setprecision(3) << m_CurrentMousePosition.x << ',' << m_CurrentMousePosition.y;
	ImGui::Text(std::string("Mouse Position: (" + mousePos.str() + ")").c_str());

	curPos << std::fixed << std::setprecision(3) << m_ControlPoint_t->position.x << ',' << m_ControlPoint_t->position.y;
	tPos << std::fixed << "(t = " << std::setprecision(3) << m_Current_t << ")";
	ImGui::Text(std::string("Current Position " + tPos.str() + ": (" + curPos.str() + ")").c_str());

	if (m_CurrentGraphMethod == GRAPH_METHOD::MID_POINT)
	{
		ImGui::Text(std::string("Point Sampling : " + std::to_string(m_MainGraph->GetPointList().size())).c_str());
	}
	// --------------------------------------------------------------

	if (ImGui::DragInt("Degree", &degree, 1, 1, 100)) { UpdateDegree(); }
	ImGui::SameLine();
	if (ImGui::SmallButton("+")) { if (degree < 100) { degree++; } UpdateDegree(); }
	ImGui::SameLine();
	if (ImGui::SmallButton("-")) { if (degree > 1) { degree--; } UpdateDegree(); }


	if (ImGui::SliderFloat("t", &m_Current_t, 0.0f, 1.0f))
	{
		PlotGraph();
	}
	if (m_CurrentGraphMethod == GRAPH_METHOD::MID_POINT)
	{
		if (ImGui::SliderInt("k", &m_Current_k, 0, 10))
		{
			PlotGraph();
		}
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
		if (ImGui::CollapsingHeader("Graphing Method"))
		{
			ImGui::Text(std::string("Current Method: " + GetGraphingMethod()).c_str());

			if (ImGui::Button("BB", ImVec2(100, 0)))
			{
				m_CurrentGraphMethod = GRAPH_METHOD::BB;
				PlotGraph();
			}
			ImGui::SameLine();
			if (ImGui::Button("NLI", ImVec2(100, 0)))
			{
				m_CurrentGraphMethod = GRAPH_METHOD::NLI;
				PlotGraph();
			}
			ImGui::SameLine();
			if (ImGui::Button("MID POINT", ImVec2(100, 0)))
			{
				m_CurrentGraphMethod = GRAPH_METHOD::MID_POINT;
				PlotGraph();
			}
		}

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

void Project8::InitGraph()
{
	// Add new Graph
	m_DraftLineGraph = m_RefGraphTable->AddGraph("Bezier Line", { 1.0f, 0.0f, 0.0f });
	m_DraftLineGraph->SetThickness(2.0f);
	m_MainGraph = m_RefGraphTable->AddGraph("Bezier Curves", { 0.0f, 1.0f, 0.0f });

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
void Project8::PlotGraph()
{
	// Clear existing Graph first
	m_DraftLineGraph->Clear();
	m_MainGraph->Clear();

	for (int i = 0; i <= degree; i++)
	{
		m_DraftLineGraph->Plot(m_ControlPoints[i]->position);
	}

	// Plot Graph
	// Using BB(Berstein Polynomials) Method
	if (m_CurrentGraphMethod == GRAPH_METHOD::BB)
	{
		// Loop through t (more step_t more precision)
		for (float t = 0.0f; t <= m_Current_t; t += 0.001f)
		{
			glm::vec3 finalPos = { 0.0f, 0.0f, 0.0f };
			for (int i = 0; i <= degree; i++)
			{
				finalPos += m_ControlPoints[i]->position * BernsteinPolynomial(i, degree, t);
			}
			m_MainGraph->Plot(finalPos);
		}
	}
	// Using NLI(Nested Linear Interpolation) Method
	else if (m_CurrentGraphMethod == GRAPH_METHOD::NLI)
	{
		std::vector<glm::vec2> positionList(degree + 1);
		// Update Coefficients (aka. Base) to std::vector 
		for (int i = 0; i <= degree; i++)
		{
			positionList[i] = m_ControlPoints[i]->position;
		}

		for (float t = 0.0f; t <= m_Current_t; t += 0.001f)
		{
			m_MainGraph->Plot(NestedLinearInterpolation(positionList, t));
		}
		std::vector<std::vector<glm::vec2>> shellsList;
		NestedLinearInterpolation(positionList, m_Current_t, shellsList);
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
	// Using Mid Point Method
	else if (m_CurrentGraphMethod == GRAPH_METHOD::MID_POINT)
	{
		std::vector<glm::vec2> positionList(degree + 1);
		// Update Coefficients (aka. Base) to std::vector 
		for (int i = 0; i <= degree; i++)
		{
			positionList[i] = m_ControlPoints[i]->position;
		}

		positionList = MidPointSubDivision(positionList, m_Current_k);

		for (int i = 0; i < std::max((int)((positionList.size() - 1) * m_Current_t + 1), 1); i++)
		{
			m_MainGraph->Plot(positionList[i]);
		}
	}
	m_ControlPoint_t->position = m_MainGraph->GetPointList().back();
}

int* Project8::UpdateDegree()
{
	while (degree > m_ControlPoints.size() - 1)
		m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(0.0f, 0.0f), m_ControlPoints[0]->color));
	while (degree < m_ControlPoints.size() - 1)
	{
		m_RefDragPointController->RemoveDragPoint(m_ControlPoints.back());
		m_ControlPoints.pop_back();
	}

	PlotGraph();

	return &degree;
}