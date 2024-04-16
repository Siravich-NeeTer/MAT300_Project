#include "Camera.h"

glm::vec2 ConvertMouseToNDC(const Window& window, const glm::vec2 inputCoord)
{
	return glm::vec2(inputCoord.x / (window.GetWidth() / 2.0f) - 1.0f, -1.0f * (inputCoord.y / (window.GetHeight() / 2.0f) - 1.0f));
}
glm::vec3 ConvertMouseToWorldSpace(const Window& window, const Camera& camera, const glm::vec2& inputCoord)
{
	// inputCoord
	// x = [-1, 1]
	// y = [-1, 1]

	glm::vec4 mouseClip = glm::vec4(inputCoord.x, inputCoord.y, -1.0f, 1.0f);
	glm::vec4 mouseEye = glm::inverse(camera.GetPerspective(window)) * mouseClip;
	mouseEye.z = -1.0f;
	mouseEye.w = 0.0f;

	glm::vec4 mouseWorld = glm::inverse(camera.GetViewMatrix()) * mouseEye;
	mouseWorld = camera.GetPosition().z * mouseWorld;
	mouseWorld = glm::vec4(camera.GetPosition().x, camera.GetPosition().y, 0.0f, 0.0f) + mouseWorld;

	return glm::vec3(mouseWorld.x, mouseWorld.y, 0.0f);
}