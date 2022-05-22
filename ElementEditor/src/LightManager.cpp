#include "LightManager.h"

#include "../Chunk.h"

LightManager::LightManager()
	: directionalLightAngleX(0.0f),
	directionalLightAngleY(0.0f),
	directionalLightColor(0.5f, 0.5f, 0.5f),
	ambientLightColor(0.5f, 0.5f, 0.5f),
	renderPositionOffset(Chunk::HALF_BLOCK_WIDTH),
	previewLightEnabled(false)
{
	previewLight = new Light(glm::vec3(1.0f, 1.0f, 1.0f), Point3di{ 0, 0, 0 }, renderPositionOffset, 1.0f);
	rebuildDirectionalLightProperties();
}

LightManager::~LightManager() {
	for (Light* light : lights) {
		if (light != nullptr)
			delete light;
	}

	if (previewLight != nullptr)
		delete previewLight;
}

void LightManager::addLight(const glm::vec3& color, Point3di& position, float strength) {
	if (lights.size() < MAX_NUM_LIGHTS) {
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
	return nullptr;
}

std::vector<Light*> LightManager::getLights() {
	if (previewLightEnabled) {
		std::vector<Light*> allLights(lights);
		allLights.push_back(previewLight);
		return allLights;
	}
	return lights;
}

float LightManager::getDirectionalLightAngleX() {
	return directionalLightAngleX;
}

void LightManager::setDirectionalLightAngleX(float angle) {
	directionalLightAngleX = angle;
	rebuildDirectionalLightProperties();
}

float LightManager::getDirectionalLightAngleY() {
	return directionalLightAngleY;
}

void LightManager::setDirectionalLightAngleY(float angle) {
	directionalLightAngleY = angle;
	rebuildDirectionalLightProperties();
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

glm::mat4 LightManager::getDirectionalLightViewMatrix() {
	return directionalLightViewMatrix;
}

glm::vec3 LightManager::getDirectionalLightVector() {
	return directionalLightVector;
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

void LightManager::updatePreviewLight(const glm::vec3& color, Point3di& position, float strength) {
	previewLight->setColor(color);
	previewLight->setBlockPosition(position);
	previewLight->setStrength(strength);
	setPreviewLightEnabled(true);
}

void LightManager::setPreviewLightEnabled(bool enabled) {
	previewLightEnabled = enabled;
}

void LightManager::rebuildDirectionalLightProperties() {
	// TODO
	/*directionalLightViewMatrix = glm::lookAt(
		directionalLightPosition,
		directionalLightTarget,
		glm::vec3(0.0f, 1.0f, 0.0f));

	directionalLightVector = glm::normalize(directionalLightTarget - directionalLightPosition);*/
}