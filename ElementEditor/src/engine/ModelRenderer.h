#pragma once

#include "Renderable.h"
#include "Camera.h"
#include "Shader.h"
#include "../Chunk.h"

class ModelRenderer {
public:
	ModelRenderer(unsigned int renderRegionWidth, unsigned int renderRegionHeight);
	~ModelRenderer();

	void render(std::vector<Chunk>& chunks, Camera& camera);
	void renderPreview(std::vector<Chunk>& previewChunks, Camera& camera);

private:
	unsigned int SHADOWMAP_WIDTH = 1024;
	unsigned int SHADOWMAP_HEIGHT = 1024;	// TODO move to class/project constants
	unsigned int renderRegionWidth, renderRegionHeight;
	unsigned int shadowMapFBO;
	unsigned int depthMapTextureId;
	Shader shadowMapShader;
	Shader debugQuadShader;

	void renderWithTransparency(std::vector<Chunk>& chunks, Camera& camera, float alpha);


	void renderQuad();
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
};