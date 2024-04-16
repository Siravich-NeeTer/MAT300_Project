#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform float u_CameraZPos;
uniform float u_PointSize;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0f);
	gl_PointSize = 75.0f / u_CameraZPos;
}