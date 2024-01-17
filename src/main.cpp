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

// Screen Size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool isCameraMove = false;
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -1.0f);

int main()
{
	srand(time(nullptr));

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "MAT300_Project_1");
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
	GraphTable graph(-3, 3);
	Project1 project1(graph);

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
		project1.Update(window, cam, dt);

		// ------- Render Graph & Text --------
		dragPointController->Render(window, cam);
		graph.Render(window, cam);
		textRenderer->RenderText(window, cam);
		// ------------ Render GUI ------------
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
	textRenderer->Clean();
	dragPointController->Clean();
	window.Destroy();
}