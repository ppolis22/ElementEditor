#pragma once

#include "Renderable.h"
#include "Camera.h"
#include "Shader.h"

class ModelRenderer {
public:
	ModelRenderer(unsigned int renderRegionWidth, unsigned int renderRegionHeight);
	~ModelRenderer();

	void render(std::vector<Renderable*> renderables, Shader& meshShader, Camera& camera);
	void renderPreview(std::vector<Renderable*> renderables, Shader& meshShader, Camera& camera);

private:
	unsigned int SHADOWMAP_WIDTH = 1024;
	unsigned int SHADOWMAP_HEIGHT = 1024;	// TODO move to class/project constants
	unsigned int renderRegionWidth, renderRegionHeight;
	unsigned int shadowMapFBO;
	unsigned int depthMapTextureId;
	Shader shadowMapShader;

	void renderWithAlpha(std::vector<Renderable*> renderables, Shader& meshShader, Camera& camera, float alpha);
};