#include "GraphTable.h"

GraphTable::GraphTable(int yMin, int yMax)
	: m_TableShader("TableShader")
{
	// Initialize Base-Graph
	float offset = 0.1f;
	//std::vector<glm::vec2> tableVertices = { {0, yMin}, {0, yMax} };
	m_TableVertices.push_back({  0.0f, yMin - offset });
	m_TableVertices.push_back({  0.0f, yMax + offset });

	m_TableVertices.push_back({ -offset, 0.0f });
	m_TableVertices.push_back({ 1.0f * tScale + offset, 0.0f });

	// Initialize Graph on Y-Axis
	for (int i = yMin; i <= yMax; i++)
	{
		TextRenderer::GetInstance()->AddText(std::to_string(i), { -0.2f, i }, 0.2f, { 1.0f, 0.0f, 0.0f });
		m_TableVertices.push_back({ -0.05f, i });
		m_TableVertices.push_back({  0.05f, i });
	}
	// Initialize Graph on T-Axis (X-Axis)
	m_TableVertices.push_back({ 1.0f * tScale,  0.05f });
	m_TableVertices.push_back({ 1.0f * tScale, -0.05f });
	TextRenderer::GetInstance()->AddText("1", {1 * tScale + offset, 0.0f}, 0.2f, {1.0f, 0.0f, 0.0f});

	m_Table_VAO.Bind();
	m_Table_VBO.BufferData(2 * sizeof(float) * m_TableVertices.size(), m_TableVertices.data(), false);

	m_Table_VAO.Attribute(m_Table_VBO, 0, 2, GL_FLOAT, 2 * sizeof(float), 0);

	m_Table_VBO.UnBind();

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

void GraphTable::Render(const Window& window, const Camera& camera)
{
	m_TableShader.Activate();
	m_TableShader.SetMat4("u_View", camera.GetViewMatrix());
	m_TableShader.SetMat4("u_Projection", camera.GetPerspective(window));
	m_TableShader.SetMat4("u_Model", glm::scale(glm::mat4(1.0f), glm::vec3(tScale, 1.0f, 1.0f)));

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
	// TODO Fix ERROR
	/*
	std::vector<Graph*>::iterator it = std::remove(m_Graphs.begin(), m_Graphs.end(), *graph);
	if (it == m_Graphs.end())
		return false;
	
	m_Graphs.erase(it, m_Graphs.end());
	*/
	return true;
}