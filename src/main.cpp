#include <iostream>
#include <string>

#include "Window.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Mesh.h"
#include "Renderer/TextRenderer.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "GraphTable.h"
#include "CustomMath.h"
#include "DragPointController.h"

#include "Project1.h"
#include "Project2.h"
#include "Project3.h"
#include "Project4.h"
#include "Project5.h"
#include "Project6.h"

// Screen Size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool isCameraMove = false;
IProject *BaseProject = nullptr;
void CleanBaseProject()
{
	if (BaseProject)
	{
		delete BaseProject;
		BaseProject = nullptr;
	}
}

int main()
{
	srand(time(nullptr));

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "MAT300_Project");
	window.Init();

	if (window.IsNull())
	{
		std::cerr << "Error(WINDOW_NULL) : Cannot open window \n";
		return -1;
	}

	TextRenderer* textRenderer = TextRenderer::GetInstance();
	DragPointController* dragPointController = DragPointController::GetInstance();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	Camera cam(glm::vec3(2, 0, 5));
	GraphTable graphTable;

	// --------------- Game Loop ---------------
	float prevTime = 0.0f;
	float dt = 0.0f;
	while (!window.ShouldClose())
	{
		float currentTime = glfwGetTime();
		dt = currentTime - prevTime;
		window.PollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Update Camera
		if (isCameraMove)
			cam.ProcessMousesMovement();
		cam.Input(dt);

		dragPointController->Update(window, cam, dt);
		
		if(BaseProject)
			BaseProject->Update(window, cam, dt);

		// ------- Render Graph & Text --------
		dragPointController->Render(window, cam);
		graphTable.Render(window, cam);
		textRenderer->RenderText(window, cam);
		// ------------ Render GUI ------------
		ImGui::Begin("Project Selector");
		if (ImGui::Button("Project 1"))
		{
			CleanBaseProject();
			BaseProject = new Project1(graphTable);
		}
		if (ImGui::Button("Project 2"))
		{
			CleanBaseProject();
			BaseProject = new Project2(graphTable);
		}
		if (ImGui::Button("Project 3"))
		{
			CleanBaseProject();
			BaseProject = new Project3(graphTable);
		}
		if (ImGui::Button("Project 4"))
		{
			CleanBaseProject();
			BaseProject = new Project4(graphTable);
		}
		if (ImGui::Button("Project 5"))
		{
			CleanBaseProject();
			BaseProject = new Project5(graphTable);
		}
		if (ImGui::Button("Project 6"))
		{
			CleanBaseProject();
			BaseProject = new Project6(graphTable);
		}

		if (ImGui::CollapsingHeader("Graph Table Component"))
		{
			if (ImGui::Checkbox("Hide Text", &graphTable.IsHideText()))
			{
				graphTable.SetTextActive(!graphTable.IsHideText());
			}
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// ------------------------------------

		// Post - Render
		Input::EndFrame();
		window.SwapBuffers();

		prevTime = currentTime;
	}

	// --------------- End Program ---------------
	// Clean Application & Window
	CleanBaseProject();
	textRenderer->Clean();
	dragPointController->Clean();
	window.Destroy();
}