#include "Project2.h"

Project2::Project2(GraphTable& graphTable)
	: IProject(graphTable)
{
	// Init GraphTable
	graphTable.InitTable(GraphTable::TableType::XY_TABLE, { -10.0f, 10.0f }, {-10.0f, 10.0f});
	m_CurrentGraphMethod = GRAPH_METHOD::BB;

	// Set DragPointController Type
	m_RefDragPointController->SetDragType(DragPoint::DragType::DRAG_XY);
	
	// Initialize
	InitGraph();
	PlotGraph();
}
Project2::~Project2()
{
	m_RefGraphTable->RemoveGraph(m_MainGraph);
	m_RefDragPointController->ClearAllDragPoint();
}

void Project2::Update(Window& window, Camera& camera, const float& dt)
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

	UpdateUI();
}
void Project2::UpdateUI()
{
	ImGui::Begin("Project_2");

	// -------------------- Print Mouse Position --------------------
	std::stringstream mousePos, curPos;
	mousePos << std::fixed << std::setprecision(3) << m_CurrentMousePosition.x << ',' << m_CurrentMousePosition.y;
	ImGui::Text(std::string("Mouse Position: (" + mousePos.str() + ")").c_str());
	curPos << std::fixed << std::setprecision(3) << m_ControlPoint_t->position.x << ',' << m_ControlPoint_t->position.y;
	ImGui::Text(std::string("Current Position: (" + curPos.str() + ")").c_str());
	// --------------------------------------------------------------

	if (ImGui::DragInt("Degree", &degree, 1, 1, 100)) { UpdateDegree(); }
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
	if (ImGui::Checkbox("Hide Polyine", &isHidePolyLine))
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

void Project2::InitGraph()
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
void Project2::PlotGraph()
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
			glm::vec2 finalPos = { 0.0f, 0.0f };
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
	}
	m_ControlPoint_t->position = m_MainGraph->GetPointList().back();
}

int* Project2::UpdateDegree()
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