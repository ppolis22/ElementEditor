#pragma once

#include "UIRenderer.h"
#include "../Mesh.h"
#include "../MeshBuilder2d.h"
#include "../MeshBuilderTextured2d.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Camera.h"

class BasicUIRenderer : public UIRenderer {
public:
	BasicUIRenderer() = delete;
	BasicUIRenderer(MeshBuilder2d& meshBuilder, MeshBuilderTextured2d& texturedMeshBuilder, int windowWidth, int windowHeight);
	virtual ~BasicUIRenderer();

	virtual void renderStaticColoredQuad(float x, float y, float width, float height, glm::vec3 color, float alpha) override;
	virtual void renderStaticTexturedQuad(float x, float y, float width, float height, const std::string& texturePath, glm::vec3 blendColor) override;
	virtual void renderTexturedQuadInScene(float x, float y, float z, float width, float height, Camera& camera,
		const std::string& texturePath, glm::vec3 blendColor) override;

protected:
	static const int TEX_SLOT_COUNT = 16;

	Mesh coloredQuad, texturedQuad;
	Shader coloredShader, texturedShader;
	int windowWidth, windowHeight;
	std::vector<Texture*> textures;		// replace with array?
	int nextSlot = 0;

	void renderTexturedQuad(const std::string& texturePath, glm::vec3 blendColor, glm::mat4 transformationMatrix);
	virtual glm::mat4 buildTransformationMatrix(float x, float y, float width, float height);
	virtual int getTextureSlot(const std::string& texturePath);
};