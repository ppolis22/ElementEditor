#version 330 core

layout(location = 0) out vec4 color;

in VS_OUT {
	vec3 worldPosition;
	vec3 vertexNormal;
	vec3 vertexColor;
	vec4 worldPositionLightSpace;
	vec3 toLightVector;
	float occlusion;
} fs_in;

uniform sampler2D shadowMap;

uniform vec3 lightColor;
uniform float alpha;

float calculateShadowPercent(vec4 worldPositionLightSpace)
{
	// perform perspective divide to arrive in NDC (technically not needed in ortho projections)
	vec3 projCoords = worldPositionLightSpace.xyz / worldPositionLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;	// from NDC to range [0,1] to sample texture
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.01 * (1.0 - dot(fs_in.vertexNormal, fs_in.toLightVector)), 0.001);	// to reduce shadow banding
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main() {
	float ambientPercent = 0.5 * (1.0 - fs_in.occlusion);
	vec3 ambientLight = ambientPercent * lightColor;
	float diffusePercent = max(dot(fs_in.vertexNormal, fs_in.toLightVector), 0.0);
	vec3 diffuseLight = diffusePercent * lightColor;

	float shadow = calculateShadowPercent(fs_in.worldPositionLightSpace);

	vec3 totalColor = (ambientLight + (1.0 - shadow) * diffuseLight) * fs_in.vertexColor;
	color = vec4(totalColor, alpha);
};