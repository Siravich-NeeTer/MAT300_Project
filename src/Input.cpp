#include "Input.h"

namespace Input
{
	float mouseX;
	float mouseY;
	float scrollOffset;
	float prevMouseX;
	float prevMouseY;
	float deltaMouseX;
	float deltaMouseY;
	bool keyPressed[GLFW_KEY_LAST];
	bool keyBeginPressed[GLFW_KEY_LAST];
	bool keyEndPressed[GLFW_KEY_LAST];

	bool IsKeyPressed(const int& key)
	{
		return keyPressed[key];
	}
	bool IsKeyBeginPressed(const int& key)
	{
		return keyBeginPressed[key];
	}
	bool IsKeyEndPressed(const int& key)
	{
		return keyEndPressed[key];
	}

	void EndFrame()
	{
		scrollOffset = 0.0f;
		deltaMouseX = 0.0f;
		deltaMouseY = 0.0f;

		for (int idx = 0; idx < GLFW_KEY_LAST; idx++)
		{
			keyBeginPressed[idx] = false;
			keyEndPressed[idx] = false;
		}
	}

	// ----------------- Callback Function -----------------
	void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// In case of : FN + F1 to FN + F12
		if (key < 0 || key >= GLFW_KEY_LAST) { return; }

		if (action == GLFW_PRESS)
		{
			keyPressed[key] = true;
			keyBeginPressed[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keyEndPressed[key] = true;
			keyPressed[key] = false;
			keyBeginPressed[key] = false;
		}

		if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	void cursorCallBack(GLFWwindow* window, double xPos, double yPos)
	{
		deltaMouseX = -(xPos - mouseX);
		deltaMouseY = -(yPos - mouseY);

		prevMouseX = mouseX;
		prevMouseY = mouseY;
		
		mouseX = (float)xPos;
		mouseY = (float)yPos;
	}
	void mouseCallBack(GLFWwindow* window, int key, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			keyPressed[key] = true;
			keyBeginPressed[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keyPressed[key] = false;
			keyBeginPressed[key] = false;
		}
	}
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		scrollOffset = yoffset;
	}
}
