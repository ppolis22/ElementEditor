#version 330 core

layout(location = 0) out vec4 color;

in vec3 vertexColor;
in vec3 vertexNormal;
in vec3 toLightVector;

uniform vec3 lightColor;
uniform float alpha;

void main() {
	float ambientPercent = 0.25;
	vec3 ambientLight = ambientPercent * lightColor;
	float diffusePercent = max(dot(vertexNormal, toLightVector), 0.0);
	vec3 diffuseLight = diffusePercent * lightColor;

	vec3 totalColor = (ambientLight + diffuseLight) * vertexColor;
	color = vec4(totalColor, alpha);
};