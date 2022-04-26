#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 inputColor;
layout(location = 3) in float occlusion;

out VS_OUT {
	vec3 worldPosition;
	vec3 vertexNormal;
	vec3 vertexColor;
	vec4 worldPositionLightSpace;
	vec3 toLightVector;
	float occlusion;
} vs_out;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightPosition;

void main() {
	vs_out.worldPosition = vec3(modelMatrix * vec4(position, 1.0));
	vs_out.vertexNormal = normalize((modelMatrix * vec4(normal, 0.0)).xyz);
	// use this instead if scaling needs to be supported, though it's expensive and should be passed in as a uniform
	//vs_out.vertexNormal = normalize( mat3(transpose(inverse(modelMatrix))) * normal );
	vs_out.worldPositionLightSpace = lightSpaceMatrix * vec4(vs_out.worldPosition, 1.0);
	vs_out.vertexColor = inputColor;
	vs_out.toLightVector = normalize(lightPosition - vs_out.worldPosition);
	vs_out.occlusion = occlusion;
	gl_Position = projectionMatrix * viewMatrix * vec4(vs_out.worldPosition, 1.0);
};