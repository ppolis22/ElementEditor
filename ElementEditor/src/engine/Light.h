#pragma once

#include "Point3d.h"
#include "../vendor/glm/glm.hpp"

struct Light {
public:
	Light(glm::vec3 color, Point3di blockPosition, glm::vec3 renderPositionOffset, float strength);

	glm::vec3 getColor();
	void setColor(glm::vec3 color);

	Point3di getBlockPosition();
	void setBlockPosition(const Point3di& position);

	float getStrength();
	void setStrength(float strength);

	glm::vec3 getRenderPosition();

private:
	glm::vec3 color;
	Point3di blockPosition;
	glm::vec3 renderPositionOffset;
	glm::vec3 renderPosition;
	float strength;
};