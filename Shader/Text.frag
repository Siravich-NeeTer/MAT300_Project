#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;

void main()
{
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(u_Text, TexCoords).r);
	FragColor = vec4(u_TextColor, 1.0f) * sampled;
}