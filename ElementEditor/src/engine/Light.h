#pragma once

#include "../vendor/glm/glm.hpp"

struct Light {
	glm::vec3 color;
	glm::vec3 position;
	float strength = 1.0f;
};