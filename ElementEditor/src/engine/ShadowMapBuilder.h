#pragma once

#include "Shader.h"
#include "DirectionalLight.h"
#include "Renderable.h"

class ShadowMapBuilder {
public:
	ShadowMapBuilder(DirectionalLight* directionalLight);

	void buildShadowMap(std::vector<Renderable*> renderables);
	unsigned int getShadowMapTextureId();

private:
	static const unsigned int SHADOWMAP_WIDTH;
	static const unsigned int SHADOWMAP_HEIGHT;

	unsigned int shadowMapFBO;
	unsigned int shadowMapTextureId;

	Shader shadowMapShader;
	DirectionalLight directionalLight;
};