#include "GraphTable.h"

GraphTable::GraphTable()
	: m_TableShader("TableShader")
{
	// Create Shader
	m_TableShader.AttachShader(BaseShader("Shader/GraphTable.vert"));
	m_TableShader.AttachShader(BaseShader("Shader/GraphTable.frag"));
	m_TableShader.LinkProgram();
}
GraphTable::~GraphTable()
{
	for (int idx = m_Graphs.size() - 1; idx >= 0; idx--)
	{
		delete m_Graphs[idx];
	}
}

void GraphTable::InitTable(TableType tableType, const glm::vec2& verticalSize, const glm::vec2& horizontalSize)
{
	m_CurrentTableType = tableType;

	// Initialize Base-Graph
	float offset = 0.1f;

	float yMin = verticalSize.x;
	float yMax = verticalSize.y;
	float xMin = horizontalSize.x;
	float xMax = horizontalSize.y;

	// Draw Base-Line
	// - Vertical Line
	m_TableVertices.push_back({ 0.0f, yMin - offset, 0.0f });
	m_TableVertices.push_back({ 0.0f, yMax + offset, 0.0f });
	// - Horizontal Line
	if (tableType == TableType::YT_TABLE)
	{
		m_TableVertices.push_back({ -offset, 0.0f, 0.0f });
		m_TableVertices.push_back({ tScale + offset, 0.0f, 0.0f });
	}
	else
	{
		m_TableVertices.push_back({ xMin - offset, 0.0f, 0.0f });
		m_TableVertices.push_back({ xMax + offset, 0.0f, 0.0f });
	}

	// Initialize Graph on Y-Axis
	for (int i = yMin; i <= yMax; i++)
	{
		if (i == 0)
			continue;

		m_TextGraphList.push_back(TextRenderer::GetInstance()->AddText(std::to_string(i), { -0.2f, i }, 0.2f, { 1.0f, 0.0f, 0.0f }));
		m_TableVertices.push_back({ -0.05f, i, 0.0f });
		m_TableVertices.push_back({ 0.05f, i, 0.0f });
	}
	
	// Initialize Graph on Horizontal
	// - Initialize Graph on T-Axis
	if (tableType == TableType::YT_TABLE)
	{
		m_TableVertices.push_back({ tScale, -0.05f, 0.0f });
		m_TableVertices.push_back({ tScale,  0.05f, 0.0f });
		m_TextGraphList.push_back(TextRenderer::GetInstance()->AddText("1", { 1 * tScale + offset, 0.0f }, 0.2f, { 1.0f, 0.0f, 0.0f }));
	}
	// - Initialize Graph on X-Axis
	else
	{
		for (int i = xMin; i <= xMax; i++)
		{
			if (i == 0)
				continue;

			m_TextGraphList.push_back(TextRenderer::GetInstance()->AddText(std::to_string(i), { i, -0.2f }, 0.2f, { 1.0f, 0.0f, 0.0f }));
			m_TableVertices.push_back({ i, -0.05f, 0.0f });
			m_TableVertices.push_back({ i, 0.05f, 0.0f });
		}
	}

	// Bind with Buffer
	m_Table_VAO.Bind();
	m_Table_VBO.BufferData(3 * sizeof(float) * m_TableVertices.size(), m_TableVertices.data(), false);

	m_Table_VAO.Attribute(m_Table_VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

	m_Table_VBO.UnBind();
}
void GraphTable::InitTable(TableType tableType, const glm::vec2& yAxisSize, const glm::vec2& xAxisSize, const glm::vec2& zAxisSize)
{
	m_CurrentTableType = tableType;

	// Initialize Base-Graph
	float offset = 0.1f;

	float yMin = yAxisSize.x;
	float yMax = yAxisSize.y;
	float xMin = xAxisSize.x;
	float xMax = xAxisSize.y;
	float zMin = zAxisSize.x;
	float zMax = zAxisSize.y;

	// Draw Base-Line
	// - Vertical Line
	m_TableVertices.push_back({ 0.0f, yMin - offset, 0.0f });
	m_TableVertices.push_back({ 0.0f, yMax + offset, 0.0f });
	// - Horizontal Line
	m_TableVertices.push_back({ xMin - offset, 0.0f, 0.0f });
	m_TableVertices.push_back({ xMax + offset, 0.0f, 0.0f });
	// - Z-Axis Line
	m_TableVertices.push_back({ 0.0f, 0.0f, zMin - offset });
	m_TableVertices.push_back({ 0.0f, 0.0f, zMax + offset });

	// Initialize Graph on Y-Axis
	for (int i = yMin; i <= yMax; i++)
	{
		if (i == 0)
			continue;

		m_TextGraphList.push_back(TextRenderer::GetInstance()->AddText(std::to_string(i), { -0.2f, i }, 0.2f, { 1.0f, 0.0f, 0.0f }));
		m_TableVertices.push_back({ -0.05f, i, 0.0f });
		m_TableVertices.push_back({ 0.05f, i, 0.0f });
	}

	// Initialize Graph on Horizontal
	for (int i = xMin; i <= xMax; i++)
	{
		if (i == 0)
			continue;

		m_TextGraphList.push_back(TextRenderer::GetInstance()->AddText(std::to_string(i), { i, -0.2f }, 0.2f, { 1.0f, 0.0f, 0.0f }));
		m_TableVertices.push_back({ 0.0f, -0.05f, i });
		m_TableVertices.push_back({ 0.0f, 0.05f, i });
	}


	// Initialize Graph on Z-Axis
	for (int i = xMin; i <= xMax; i++)
	{
		if (i == 0)
			continue;

		m_TextGraphList.push_back(TextRenderer::GetInstance()->AddText(std::to_string(i), { -0.2f, 0.0f, i }, 0.2f, { 1.0f, 0.0f, 0.0f }));
		m_TableVertices.push_back({ i, -0.05f, 0.0f });
		m_TableVertices.push_back({ i, 0.05f, 0.0f });
	}

	// Bind with Buffer
	m_Table_VAO.Bind();
	m_Table_VBO.BufferData(3 * sizeof(float) * m_TableVertices.size(), m_TableVertices.data(), false);

	m_Table_VAO.Attribute(m_Table_VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

	m_Table_VBO.UnBind();
}

void GraphTable::Render(const Window& window, const Camera& camera)
{
	if (m_TableVertices.empty())
		return;

	m_TableShader.Activate();
	m_TableShader.SetMat4("u_View", camera.GetViewMatrix());
	m_TableShader.SetMat4("u_Projection", camera.GetPerspective(window));
	if(m_CurrentTableType == TableType::YT_TABLE)
		m_TableShader.SetMat4("u_Model", glm::scale(glm::mat4(1.0f), glm::vec3(tScale, 1.0f, 1.0f)));
	else
		m_TableShader.SetMat4("u_Model", glm::mat4(1.0f));

	//glLineWidth(20.0f / camera.GetPosition().z);
	for (Graph* &graph : m_Graphs)
	{
		graph->Render(m_TableShader);
	}

	glLineWidth(4.0f);
	m_TableShader.SetMat4("u_Model", glm::mat4(1.0f));
	m_TableShader.SetVec3("u_Color", glm::vec3(0.0f));
	m_Table_VAO.Bind();
	glDrawArrays(GL_LINES, 0, m_TableVertices.size());
	
}

Graph* GraphTable::AddGraph(const std::string& name, const glm::vec3& color)
{
	m_Graphs.push_back(new Graph(name, color));
	return m_Graphs.back();
}
bool GraphTable::RemoveGraph(Graph* graph)
{
	auto it = std::find(m_Graphs.begin(), m_Graphs.end(), graph);
	if (it == m_Graphs.end())
		return false;

	m_Graphs.erase(it);

	return true;
}

void GraphTable::Reset()
{
	TextRenderer::GetInstance()->Reset();
	m_TableVertices.clear();

	for (int idx = m_Graphs.size() - 1; idx >= 0; idx--)
	{
		delete m_Graphs[idx];
	}
	m_Graphs.clear();
}
void GraphTable::SetTextActive(bool active)
{
	for (int idx = m_TextGraphList.size() - 1; idx >= 0; idx--)
	{
		m_TextGraphList[idx]->active = active;
	}
}