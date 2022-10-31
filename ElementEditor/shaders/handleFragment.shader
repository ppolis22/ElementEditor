#version 330 core

layout(location = 0) out vec4 color;

in vec3 vertexColor;

uniform float alpha;

void main() {
	color = vec4(vertexColor, alpha);
};