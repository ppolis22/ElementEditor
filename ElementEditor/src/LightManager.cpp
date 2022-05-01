#include "LightManager.h"

void LightManager::addLight(Light* lightToAdd) {
	if (lightCount < MAX_NUM_LIGHTS) {
		lights.push_back(lightToAdd);
		lightCount++;
	}
}

void LightManager::removeLight(Light* lightToRemove) {
	for (Light* light : lights) {
		auto foundPos = std::find(lights.begin(), lights.end(), lightToRemove);
		if (foundPos != lights.end()) {
			lights.erase(foundPos);
			break;
		}
	}
}
void LightManager::removeLight(const Point3di& position) {
	removeLight(getLight(position));
}

static bool vec3Equal(glm::vec3 v0, glm::vec3 v1) {
	float tolerance = 0.0001f;
	return abs(v0.x - v1.x) < tolerance && abs(v0.y - v1.y) < tolerance && abs(v0.z - v1.z) < tolerance;
}

Light* LightManager::getLight(const Point3di& position) {
	glm::vec3 floatPos{ position.x, position.y, position.z };
	for (auto it = lights.begin(); it != lights.end(); ++it) {
		if (vec3Equal((*it)->position, floatPos)) {
			return *it;
		}
	}
}

std::vector<Light*> LightManager::getLights() {
	return lights;
}

glm::vec3 LightManager::getDirectionalLightPosition() {
	return directionalLightPosition;
}

void LightManager::setDirectionalLightPosition(glm::vec3 position) {
	directionalLightPosition = position;
}

glm::vec3 LightManager::getDirectionalLightColor() {
	return directionalLightColor;
}

void LightManager::setDirectionalLightColor(glm::vec3 color) {
	directionalLightColor = color;
}

bool LightManager::directionalLightIsEnabled() {
	return directionalLightEnabled;
}

void LightManager::setDirectionalLightIsEnabled(bool enabled) {
	directionalLightEnabled = enabled;
}

int LightManager::getLightCount() {
	return lightCount;
}