#include "Light.h"

Light::Light(glm::vec3 color, Point3di blockPosition, glm::vec3 renderPositionOffset, float strength)
	: color(color), renderPositionOffset(renderPositionOffset), strength(strength)
{
	setBlockPosition(blockPosition);
}

glm::vec3 Light::getColor() { return color; }

void Light::setColor(glm::vec3 color) { this->color = color; }

float Light::getStrength() { return strength; }

void Light::setStrength(float strength) { this->strength = strength; }

Point3di Light::getBlockPosition() { return blockPosition; }

void Light::setBlockPosition(const Point3di& position) {
	this->blockPosition = position;
	this->renderPosition = glm::vec3(
		this->blockPosition.x + renderPositionOffset.x,
		this->blockPosition.y + renderPositionOffset.y,
		this->blockPosition.z + renderPositionOffset.z);
}

glm::vec3 Light::getRenderPosition() { return renderPosition; }