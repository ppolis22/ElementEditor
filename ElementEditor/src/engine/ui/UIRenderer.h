#pragma once

#include "../vendor/glm/glm.hpp"

class UIRenderer {
public:
	virtual ~UIRenderer() {};

	virtual void renderQuad(float x, float y, float width, float height, glm::vec3 color, float alpha) = 0;
};