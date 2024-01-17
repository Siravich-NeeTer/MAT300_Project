#version 460 core

layout(location = 0) in vec4 aPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 TexCoords;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos.xy, 0.0f, 1.0f);
	TexCoords = aPos.zw;
}