#include "LightManager.h"

#include "editor/model/Chunk.h"

LightManager::LightManager()
	: ambientLightColor(0.5f, 0.5f, 0.5f),
	renderPositionOffset(Chunk::HALF_BLOCK_WIDTH),
	previewLightEnabled(false)
{
	previewLight = new Light(glm::vec3(1.0f, 1.0f, 1.0f), Point3di{ 0, 0, 0 }, renderPositionOffset, 1.0f);
}

LightManager::~LightManager() {
	for (Light* light : lights)
		delete light;

	delete previewLight;
	delete directionalLight;
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

void LightManager::setDirectionalLight(int xBound, int yBound, int zBound, float yAngle, float xAngle, glm::vec3 color) {
	delete directionalLight;
	directionalLight = new DirectionalLight(xBound, yBound, zBound, yAngle, xAngle, color);
}

DirectionalLight* LightManager::getDirectionalLight() {
	return directionalLight;
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