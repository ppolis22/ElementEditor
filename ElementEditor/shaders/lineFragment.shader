#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 lineColor;

void main()
{
	color = vec4(lineColor, 1.0f);
}