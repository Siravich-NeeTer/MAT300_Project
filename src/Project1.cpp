#include "Project1.h"

Project1::Project1(GraphTable& graphTable)
	: IProject(graphTable)
{
	// Init GraphTable
	graphTable.InitTable(GraphTable::TableType::YT_TABLE, { -3.0f, 3.0f });
	m_CurrentGraphMethod = GRAPH_METHOD::BB;

	// Set DragPointController Type
	m_RefDragPointController->SetDragType(DragPointController::DragType::DRAG_Y);

	// Initialize
	InitGraph();
	PlotGraph();
}
Project1::~Project1()
{
	m_RefGraphTable->RemoveGraph(m_MainGraph);
	m_RefDragPointController->ClearAllDragPoint();
}

void Project1::Update(Window& window, Camera& camera, const float& dt)
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
void Project1::UpdateUI()
{
	ImGui::Begin("Project_1");

	// -------------------- Print Mouse Position --------------------
	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << m_CurrentMousePosition.x / m_RefGraphTable->GetTableScale() << ',' << m_CurrentMousePosition.y;
	ImGui::Text(std::string("Current Position: (" + stream.str() + ")").c_str());
	// --------------------------------------------------------------

	if (ImGui::DragInt("Degree", &degree, 1, 1, 100)) { UpdateDegree(); }
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

			if (ImGui::Button("BB", ImVec2(100, 0)))
			{
				m_CurrentGraphMethod = GRAPH_METHOD::BB;
				m_MainGraph->Clear();
				PlotGraph();
			}
			ImGui::SameLine();
			if (ImGui::Button("NLI", ImVec2(100, 0)))
			{
				m_CurrentGraphMethod = GRAPH_METHOD::NLI;
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

void Project1::InitGraph()
{
	// Add new Graph
	m_MainGraph = m_RefGraphTable->AddGraph("Bernstein Polynomial", { 0.0f, 1.0f, 0.0f });
	
	// Initialize Degree = 1
	degree = 1;
	isHidePoint = false;

	// Initialize Control Points a0, a1
	m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(0.0f * m_RefGraphTable->GetTableScale(), 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(1.0f * m_RefGraphTable->GetTableScale(), 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
}
void Project1::PlotGraph()
{
	// Plot Graph
	// Using BB(Berstein Polynomials) Method
	if (m_CurrentGraphMethod == GRAPH_METHOD::BB)
	{
		// Loop through t (more step_t more precision)
		for (float t = 0.0f; t <= 1.0f; t += 0.001f)
		{
			float y = 0.0f;
			for (int i = 0; i <= degree; i++)
			{
				y += m_ControlPoints[i]->position.y * BernsteinPolynomial(i, degree, t);
			}
			m_MainGraph->Plot({ t,  y });
		}
	}
	// Using NLI(Nested Linear Interpolation) Method
	else if (m_CurrentGraphMethod == GRAPH_METHOD::NLI)
	{
		std::vector<float> coefficientList(degree + 1);
		// Update Coefficients (aka. Base) to std::vector 
		for (int i = 0; i <= degree; i++)
		{
			coefficientList[i] = m_ControlPoints[i]->position.y;
		}

		for (float t = 0.0f; t <= 1.0f; t += 0.001f)
		{
			m_MainGraph->Plot({ t, NestedLinearInterpolation(coefficientList, t)});
		}
	}
}

int* Project1::UpdateDegree()
{
	while(degree > m_ControlPoints.size() - 1)
		m_ControlPoints.push_back(m_RefDragPointController->AddDragPoint(glm::vec2(1.0f * m_RefGraphTable->GetTableScale(), 1.0f), m_ControlPoints[0]->color));
	while (degree < m_ControlPoints.size() - 1)
	{
		m_RefDragPointController->RemoveDragPoint(m_ControlPoints.back());
		m_ControlPoints.pop_back();
	}

	float step = 0.0f;
	for (int idx = 0; idx <= degree; idx++, step += 1.0f / degree * m_RefGraphTable->GetTableScale())
	{
		m_ControlPoints[idx]->position.x = step;
	}
	m_MainGraph->Clear();
	PlotGraph();

	return &degree;
}