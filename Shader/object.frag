#version 460 core

out vec4 FragColor;
in vec3 FragPos;

uniform vec3 u_Color;

void main() 
{
    FragColor = vec4(u_Color, 1.0f);
}