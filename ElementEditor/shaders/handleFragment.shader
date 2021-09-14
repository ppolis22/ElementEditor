#version 330 core

layout(location = 0) out vec4 color;

in vec3 vertexColor;
in vec3 vertexNormal;
in vec3 toLightVector;

uniform vec3 lightColor;
uniform float alpha;

void main() {
	color = vec4(vertexColor, alpha);
};