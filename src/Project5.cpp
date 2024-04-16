#include "Project5.h"

Project5::Project5(GraphTable& graphTable)
	: IProject(graphTable)
{
	// Init GraphTable
	graphTable.InitTable(GraphTable::TableType::YT_TABLE, { -3.0f, 3.0f });
	m_CurrentGraphMethod = GRAPH_METHOD::DE_BOOR;

	// Set DragPointController Type
	m_RefDragPointController->SetDragType(DragPoint::DragType::DRAG_Y);

	// Initialize
	InitGraph();
	PlotGraph();
}
Project5::~Project5()
{
	m_RefGraphTable->RemoveGraph(m_MainGraph);
	m_RefDragPointController->ClearAllDragPoint();
}

void Project5::Update(Window& window, Camera& camera, const float& dt)
{
	// If ControlPoints is empty (degree = 0) -> no need to update
	if (m_ControlPoints.empty())
		return;

	for (int idx = 0; idx < m_ControlPoints.size(); idx++)
	{
		// If one of ControlPoints move -> Update entire graph
		if (m_ControlPoints[idx]->isMove)
		{
			// Clear old one & Plot new one
			m_MainGraph->Clear();
			PlotGraph();
		}
	}

	// Update currentMousePosition
	m_CurrentMousePosition = ConvertMouseToWorldSpace(window, camera, ConvertMouseToNDC(window, { Input::mouseX, Input::mouseY }));

	UpdateUI();
}
void Project5::UpdateUI()
{
	ImGui::Begin("Project_5");

	// -------------------- Print Mouse Position --------------------
	std::stringstream mousePos;
	mousePos << std::fixed << std::setprecision(3) << m_CurrentMousePosition.x / m_RefGraphTable->GetTableScale() << ',' << m_CurrentMousePosition.y;
	ImGui::Text(std::string("Mouse Position: (" + mousePos.str() + ")").c_str());
	// --------------------------------------------------------------

	if (ImGui::DragInt("Degree", &degree, 1, 1, 20)) { UpdateDegree(); }
	ImGui::SameLine();
	if (ImGui::SmallButton("+")) { if (degree < 20) { if (N < 40) { degree++; N++; } else if(N - degree > degree + 2) { degree++; } } UpdateDegree(); }
	ImGui::SameLine();
	if (ImGui::SmallButton("-")) { if (degree > 1) { degree--; N--; } UpdateDegree(); }

	if (ImGui::DragInt("N", &N, 1, 3, 40)) { UpdateDegree(); }
	ImGui::SameLine();
	if (ImGui::SmallButton("N+")) { if (N < 40) { N++; } UpdateDegree(); }
	ImGui::SameLine();
	if (ImGui::SmallButton("N-")) { if (N > degree + 2 && N - degree > degree + 1) { N--; } UpdateDegree(); }

	if (ImGui::Checkbox("Hide Point", &isHidePoint))
	{
		for (int idx = 0; idx < m_ControlPoints.size(); idx++)
		{
			m_ControlPoints[idx]->isActive = !isHidePoint;
		}
	}

	// ---------------------- Graphing Method -----------------------
	if (ImGui::CollapsingHeader("Graph"))
	{
		if (ImGui::CollapsingHeader("Graphing Method"))
		{
			ImGui::Text(std::string("Current Method: " + GetGraphingMethod()).c_str());

			if (ImGui::Button("DE BOOR", ImVec2(100, 0)))
			{
				m_CurrentGraphMethod = GRAPH_METHOD::DE_BOOR;
				m_MainGraph->Clear();
				PlotGraph();
			}
			ImGui::SameLine();
			if (ImGui::Button("DIVIDED DIFFERENCE", ImVec2(150, 0)))
			{
				m_CurrentGraphMethod = GRAPH_METHOD::DIVIDED_DIFFERENCE;
				m_MainGraph->Clear();
				PlotGraph();
			}
			ImGui::SameLine();
			if (ImGui::Button("CRAMMERS RULE", ImVec2(100, 0)))
			{
				m_CurrentGraphMethod = GRAPH_METHOD::CRAMMERS;
				m_MainGraph->Clear();
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
			if (ImGui::SliderFloat(std::string("a" + std::to_string(i)).c_str(), &m_ControlPoints[i]->position.y, -3.0f, 3.0f))
			{
				m_MainGraph->Clear();
				PlotGraph();
			}
		}
	}
	// --------------------------------------------------------------

	ImGui::End();
}

void Project5::InitGraph()
{
	// Add new Graph
	m_MainGraph = m_RefGraphTable->AddGraph("De Boor Spline Function", { 0.0f, 1.0f, 0.0f });

	// Initialize Degree = 3 & N = 13
	degree = 3;
	N = 13;
	isHidePoint = false;

	// Set-up Control Points
	for (int i = 0; i < N - degree; i++)
	{
		float progress = m_RefGraphTable->GetTableScale() / N;
		m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(((degree + 1) / 2.0f + i) * progress, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		
	}
}
void Project5::PlotGraph()
{
	// Plot Graph
	std::vector<float> coefficientList(m_ControlPoints.size());
	// Update Coefficients (aka. Base) to std::vector 
	for (int i = 0; i < m_ControlPoints.size(); i++)
	{
		coefficientList[i] = m_ControlPoints[i]->position.y;
	}
	// Using De Boor Method
	if (m_CurrentGraphMethod == GRAPH_METHOD::DE_BOOR)
	{
		for (float t = degree; t <= N - degree; t += 0.001f)
		{
			m_MainGraph->Plot({ t / N, NestedLinearInterpolation_DeBoor(coefficientList, t, degree, N)});
		}
	}
	else if (m_CurrentGraphMethod == GRAPH_METHOD::DIVIDED_DIFFERENCE)
	{
		for (float t = degree; t <= N - degree; t += 0.01f)
		{
			float resultY = 0.0f;
			for (int i = 0; i <= N - degree - 1; i++)
			{
				resultY += coefficientList[i] * DividedDifference_BSpline(t, degree, N, i);
			}
			m_MainGraph->Plot({ t / N, resultY });
		}
	}
	else if (m_CurrentGraphMethod == GRAPH_METHOD::CRAMMERS)
	{
		for (float t = degree; t <= N - degree; t += 0.01f)
		{
			float resultY = 0.0f;
			for (int i = 0; i <= N - degree - 1; i++)
			{
				resultY += coefficientList[i] * Cramer_BSpline(t, degree, N, i);
			}
			m_MainGraph->Plot({ t / N, resultY });
		}
	}
}

int* Project5::UpdateDegree()
{
	while (N - degree >= m_ControlPoints.size() - 1)
		m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(1.0f * m_RefGraphTable->GetTableScale(), 1.0f), m_ControlPoints[0]->color));
	while (N - degree <= m_ControlPoints.size() - 1)
	{
		m_RefDragPointController->RemoveDragPoint(m_ControlPoints.back());
		m_ControlPoints.pop_back();
	}

	// Set-up Control Points
	for (int i = 1; i <= N - degree; i++)
	{
		float progress = m_RefGraphTable->GetTableScale() / N;
		m_ControlPoints[i - 1]->position = glm::vec3(((degree + 1) / 2.0f + (i - 1)) * progress, 1.0f, 0.0f);
	}
	m_MainGraph->Clear();
	PlotGraph();

	return &degree;
}