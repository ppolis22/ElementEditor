#pragma once

#include "Renderable.h"
#include "Camera.h"
#include "Shader.h"

class ModelRenderer {
public:
	ModelRenderer(unsigned int renderRegionWidth, unsigned int renderRegionHeight);

	void renderNoShadows(std::vector<Renderable*> renderables, Shader& meshShader, Camera& camera, float alpha);
	void renderWithShadows(std::vector<Renderable*> renderables, Shader& meshShader, Camera& camera, float alpha);

private:
	unsigned int SHADOWMAP_WIDTH = 1024;
	unsigned int SHADOWMAP_HEIGHT = 1024;	// TODO move to class/project constants

	unsigned int renderRegionWidth, renderRegionHeight;
	unsigned int shadowMapFBO;
	unsigned int depthMapTextureId;
	Shader shadowMapShader;
};