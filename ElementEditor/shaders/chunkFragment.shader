#version 330 core

#define MAX_POINT_LIGHTS 16

layout(location = 0) out vec4 color;

in VS_OUT {
	vec3 worldPosition;
	vec3 vertexNormal;
	vec3 vertexColor;
	vec4 worldPositionDirectionalLightSpace;
	vec3 pointLightContrib[MAX_POINT_LIGHTS];
	float occlusion;
} fs_in;

uniform sampler2D shadowMap;
uniform int numPointLights;
uniform vec3 directionalLightColor;
uniform vec3 toDirectionalLightVector;
uniform vec3 ambientLightColor;
uniform float alpha;

float calculateShadowPercent(vec4 worldPositionLightSpace)
{
	// perform perspective divide to arrive in NDC (technically not needed in ortho projections)
	vec3 projCoords = worldPositionLightSpace.xyz / worldPositionLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;	// from NDC to range [0,1] to sample texture
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.01 * (1.0 - dot(fs_in.vertexNormal, toDirectionalLightVector)), 0.001);	// to reduce shadow banding
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main() {
	vec3 ambientLight = (1.0 - fs_in.occlusion) * ambientLightColor;
	float directionalDiffusePercent = max(dot(fs_in.vertexNormal, toDirectionalLightVector), 0.0);
	vec3 directionalDiffuseLight = directionalDiffusePercent * directionalLightColor;

	float shadow = calculateShadowPercent(fs_in.worldPositionDirectionalLightSpace);

	vec3 pointDiffuseLight;
	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		if (i == numPointLights) {
			break;	// workaround for GLSL looping constraint
		}

		pointDiffuseLight += fs_in.pointLightContrib[i];
	}

	vec3 totalColor = (ambientLight + pointDiffuseLight + (1.0 - shadow) * directionalDiffuseLight) * fs_in.vertexColor;
	color = vec4(totalColor, alpha);
};