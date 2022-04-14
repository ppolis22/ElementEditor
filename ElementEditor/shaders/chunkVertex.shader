#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 inputColor;

out vec3 vertexColor;
out vec3 vertexNormal;
out vec3 toLightVector;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPosition;

void main() {
	vec4 worldPosition = modelMatrix * vec4(position, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	vertexNormal = normalize((modelMatrix * vec4(normal, 0.0)).xyz);

	// use this instead if scaling needs to be supported, though it's expensive and should be passed in as a uniform
	//vertexNormal = normalize( mat3(transpose(inverse(modelMatrix))) * normal );

	toLightVector = normalize(lightPosition - worldPosition.xyz);

	vertexColor = inputColor;
	//if (type == 0) {
	//	vertexColor = vec3(1.0, 0.0, 0.0);
	//} else if (type == 1) {
	//	vertexColor = vec3(0.0, 1.0, 0.0);
	//} else if (type == 2) {
	//	vertexColor = vec3(0.0, 0.0, 1.0);
	//} else if (type == 3) {
	//	vertexColor = vec3(1.0, 0.55, 0.16);
	//} else {
	//	vertexColor = vec3(0.5, 0.5, 0.5);
	//}
};