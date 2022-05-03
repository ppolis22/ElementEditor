#include "LightManager.h"

#include "../Chunk.h"

LightManager::LightManager()
	: directionalLightPosition(1.0f, 1.0f, 1.0f),
	directionalLightColor(0.5f, 0.5f, 0.5f),
	ambientLightColor(0.5f, 0.5f, 0.5f)
{}

LightManager::~LightManager() {
	for (Light* light : lights) {
		delete light;
	}
}

void LightManager::addLight(const glm::vec3& color, Point3di& position, float strength) {
	if (lights.size() < MAX_NUM_LIGHTS) {
		glm::vec3 renderPositionOffset(Chunk::HALF_BLOCK_WIDTH);
		Light* lightToAdd = new Light(color, position, renderPositionOffset, strength);
		lights.push_back(lightToAdd);
	}
}

void LightManager::removeLight(Light* lightToRemove) {
	for (Light* light : lights) {
		auto foundPos = std::find(lights.begin(), lights.end(), lightToRemove);
		if (foundPos != lights.end()) {
			delete (*foundPos);
			lights.erase(foundPos);
			break;
		}
	}
}
void LightManager::removeLight(const Point3di& position) {
	removeLight(getLight(position));
}

Light* LightManager::getLight(const Point3di& position) {
	for (auto it = lights.begin(); it != lights.end(); ++it) {
		if ((*it)->getBlockPosition() == position) {
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

glm::vec3 LightManager::getAmbientLightColor() {
	return ambientLightColor;

}
void LightManager::setAmbientLightColor(glm::vec3 color) {
	ambientLightColor = color;
}

int LightManager::getLightCount() {
	return lights.size();
}

Light* LightManager::getSelectedLight() {
	return selectedLight;
}

void LightManager::setSelectedLight(Light* light) {
	selectedLight = light;
}