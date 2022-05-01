#version 330 core

#define MAX_POINT_LIGHTS 16

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 inputColor;
layout(location = 3) in float occlusion;

out VS_OUT {
	vec3 worldPosition;
	vec3 vertexNormal;
	vec3 vertexColor;
	vec4 worldPositionDirectionalLightSpace;
	vec3 toDirectionalLightVector;
	vec3 pointLightContrib[MAX_POINT_LIGHTS];
	float occlusion;
} vs_out;

struct PointLight {
	vec3 color;
	vec3 position;
	float strength;
};

uniform PointLight lights[MAX_POINT_LIGHTS];
uniform int numPointLights;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 directionalLightSpaceMatrix;
uniform vec3 directionalLightPosition;

void main() {
	vs_out.worldPosition = vec3(modelMatrix * vec4(position, 1.0));
	vs_out.vertexNormal = normalize((modelMatrix * vec4(normal, 0.0)).xyz);
	// use this instead if scaling needs to be supported, though it's expensive and should be passed in as a uniform
	//vs_out.vertexNormal = normalize( mat3(transpose(inverse(modelMatrix))) * normal );
	vs_out.worldPositionDirectionalLightSpace = directionalLightSpaceMatrix * vec4(vs_out.worldPosition, 1.0);
	vs_out.vertexColor = inputColor;
	vs_out.toDirectionalLightVector = normalize(directionalLightPosition - vs_out.worldPosition);
	vs_out.occlusion = occlusion;

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		if (i == numPointLights) {
			break;	// workaround for GLSL looping constraint
		}

		// doing this calculation in the vertex shader is faster but less accurate than in the fragment
		// shader, possibly switch if getting strange results
		vec3 toLightVector = normalize(lights[i].position - vs_out.worldPosition);
		float diffusePercent = max(dot(vs_out.vertexNormal, toLightVector), 0.0);
		float distance = length(lights[i].position - vs_out.worldPosition);
		float attenuation = 1.0 / ( 1.0 + (distance / lights[i].strength));
		vs_out.pointLightContrib[i] = (diffusePercent * attenuation) * lights[i].color;
	}

	gl_Position = projectionMatrix * viewMatrix * vec4(vs_out.worldPosition, 1.0);
};