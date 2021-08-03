#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float type;

out vec4 vertexColor;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * vec4(position, 1);
	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
};