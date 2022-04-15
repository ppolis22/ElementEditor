#pragma once

#include "../vendor/glm/glm.hpp"
#include <string>

class UIRenderer {
public:
	virtual ~UIRenderer() {};

	virtual void renderColoredQuad(float x, float y, float width, float height, glm::vec3 color, float alpha) = 0;
	virtual void renderTexturedQuad(float x, float y, float width, float height, const std::string& texturePath, glm::vec3 blendColor) = 0;
};