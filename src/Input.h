#pragma once

#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <iostream>
#include <cstring>

namespace Input
{
	extern float mouseX;
	extern float mouseY;
	extern float scrollOffset;
	extern float prevMouseX;
	extern float prevMouseY;
	extern float deltaMouseX;
	extern float deltaMouseY;
	extern bool keyPressed[GLFW_KEY_LAST];
	extern bool keyBeginPressed[GLFW_KEY_LAST];
	extern bool keyEndPressed[GLFW_KEY_LAST];

	bool IsKeyPressed(const int& key);
	bool IsKeyBeginPressed(const int& key);
	bool IsKeyEndPressed(const int& key);

	void EndFrame();

	// ----------------- Callback Function -----------------
	void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursorCallBack(GLFWwindow* window, double xPos, double yPos);
	void mouseCallBack(GLFWwindow* window, int key, int action, int mods);
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};