#include "Project2.h"

Project2::Project2(GraphTable& graphTable)
	: IProject(graphTable)
{
	// Init GraphTable
	graphTable.InitTable(GraphTable::TableType::XY_TABLE, { -10.0f, 10.0f }, {-10.0f, 10.0f});
	m_CurrentGraphMethod = GRAPH_METHOD::BB;

	// Set DragPointController Type
	m_RefDragPointController->SetDragType(DragPointController::DragType::DRAG_XY);
}
Project2::~Project2()
{
	m_RefGraphTable->RemoveGraph(m_MainGraph);
	m_RefDragPointController->ClearAllDragPoint();
}

void Project2::Update(Window& window, Camera& camera, const float& dt)
{

}
void Project2::UpdateUI()
{

}