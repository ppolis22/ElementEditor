#pragma once

#include "engine/Point3d.h"
#include "engine/Light.h"

#include <vector>

class LightManager {

public:
	void addLight(Light* light);
	void removeLight(Light* light);
	void removeLight(const Point3di& position);
	Light* getLight(const Point3di& position);
	std::vector<Light*> getLights();
	int getLightCount();

	// Currently only one directional, shadow-casting light is supported, intended
	// to be the main light source in the scene.
	//
	// Blocks will not be lit based on their positions relative to the directional
	// light position, rather this position relative to the scene origin will determine
	// the light vector to illuminate all blocks, as well as the location of the 
	// shadow map rendering point. Thus, the position should be further from the origin
	// than everything wished to cast/receive shadows.
	glm::vec3 getDirectionalLightPosition();
	void setDirectionalLightPosition(glm::vec3 position);
	glm::vec3 getDirectionalLightColor();
	void setDirectionalLightColor(glm::vec3 color);
	bool directionalLightIsEnabled();
	void setDirectionalLightIsEnabled(bool enabled);

	const static int MAX_NUM_LIGHTS = 16;

private:
	std::vector<Light*> lights;
	glm::vec3 directionalLightPosition;
	glm::vec3 directionalLightColor;
	bool directionalLightEnabled = true;
	int lightCount = 0;

};