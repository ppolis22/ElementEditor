#pragma once

#include "../Camera.h"
#include "../vendor/glm/glm.hpp"
#include <string>

class UIRenderer {
public:
	virtual ~UIRenderer() {};

	virtual void renderStaticColoredQuad(float x, float y, float width, float height, glm::vec3 color, float alpha) = 0;
	virtual void renderStaticTexturedQuad(float x, float y, float width, float height, const std::string& texturePath, glm::vec3 blendColor) = 0;
	virtual void renderTexturedQuadInScene(float x, float y, float z, float width, float height, Camera& camera,
		const std::string& texturePath, glm::vec3 blendColor) = 0;
};