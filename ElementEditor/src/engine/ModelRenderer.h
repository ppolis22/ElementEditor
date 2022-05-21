#pragma once

#include "Renderable.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"

class ModelRenderer {
public:
	ModelRenderer(unsigned int renderRegionWidth, unsigned int renderRegionHeight);

	void renderWithShadows(
		std::vector<Renderable*> renderables,
		std::vector<Light*> pointLights,
		glm::vec3 directionalLightColor,
		glm::vec3 directionalLightVector,
		glm::mat4 directionalLightViewMatrix,
		glm::vec3 ambientLightColor,
		Shader& meshShader, 
		Camera& camera, 
		float alpha
	);

	void renderNoShadows(
		std::vector<Renderable*> renderables, 
		std::vector<Light*> pointLights,
		glm::vec3 directionalLightColor, 
		glm::vec3 directionalLightVector,
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
	unsigned int SHADOWMAP_WIDTH = 1024;
	unsigned int SHADOWMAP_HEIGHT = 1024;	// TODO move to class/project constants

	unsigned int renderRegionWidth, renderRegionHeight;
	unsigned int shadowMapFBO;
	unsigned int depthMapTextureId;

	Shader shadowMapShader;
	Shader lineShader;
};