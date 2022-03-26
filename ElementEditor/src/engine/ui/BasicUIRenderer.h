#pragma once

#include "UIRenderer.h"
#include "../Mesh.h"
#include "../MeshBuilder2d.h"
#include "../Shader.h"

class BasicUIRenderer : public UIRenderer {
public:
	BasicUIRenderer() = delete;
	BasicUIRenderer(MeshBuilder2d& meshBuilder, int windowWidth, int windowHeight);
	~BasicUIRenderer();

	virtual void renderQuad(float x, float y, float width, float height, glm::vec3 color, float alpha) override;

private:
	Mesh quad;
	Shader shader;
	int windowWidth, windowHeight;

	glm::mat4 buildTransformationMatrix(float x, float y, float width, float height);
};