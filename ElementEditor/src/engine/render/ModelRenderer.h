#pragma once

#include "Renderable.h"
#include "Camera.h"
#include "Shader.h"
#include "engine/light/Light.h"
#include "engine/light/DirectionalLight.h"

class ModelRenderer {
public:
	ModelRenderer(unsigned int renderRegionWidth, unsigned int renderRegionHeight, unsigned int shadowMapTextureId);

	void renderWithShadows(
		std::vector<Renderable*> renderables,
		std::vector<Light*> pointLights,
		DirectionalLight* directionalLight,
		glm::vec3 ambientLightColor,
		Shader& meshShader, 
		Camera& camera, 
		float alpha
	);

	void renderNoShadows(
		std::vector<Renderable*> renderables, 
		std::vector<Light*> pointLights,
		DirectionalLight* directionalLight,
		glm::vec3 ambientLightColor,
		Shader& meshShader, 
		Camera& camera, 
		float alpha
	);

	void renderNoLighting(
		std::vector<Renderable*> renderables,
		Shader& meshShader,
		Camera& camera,
		float alpha
	);

	void renderLines(
		Mesh& mesh,
		Camera& camera,
		glm::vec3 color
	);

private:
	unsigned int renderRegionWidth, renderRegionHeight;
	unsigned int shadowMapTextureId = 0;
	Shader lineShader;
};