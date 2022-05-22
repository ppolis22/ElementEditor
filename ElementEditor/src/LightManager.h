#pragma once

#include "engine/Point3d.h"
#include "engine/Light.h"

#include <vector>

class LightManager {

public:
	LightManager();
	~LightManager();

	// These methods operate specifically on a collection of point lights
	void addLight(const glm::vec3& color, Point3di& position, float strength);
	void removeLight(Light* light);
	void removeLight(const Point3di& position);
	Light* getLight(const Point3di& position);

	// Returns a vector of all the lights in the scene, plus the preview light, if enabled
	std::vector<Light*> getLights();
	int getLightCount();
	Light* getSelectedLight();
	void setSelectedLight(Light* light);

	// Currently only one directional, shadow-casting light is supported, intended
	// to be the main light source in the scene.
	//
	// Blocks will not be lit based on their positions relative to the directional
	// light position, rather this position relative to the scene origin will determine
	// the light vector to illuminate all blocks, as well as the location of the 
	// shadow map rendering point. Thus, the position should be further from the origin
	// than everything wished to cast/receive shadows.
	float getDirectionalLightAngleX();
	void setDirectionalLightAngleX(float angle);
	float getDirectionalLightAngleY();
	void setDirectionalLightAngleY(float angle);
	glm::vec3 getDirectionalLightColor();
	void setDirectionalLightColor(glm::vec3 color);
	bool directionalLightIsEnabled();
	void setDirectionalLightIsEnabled(bool enabled);
	glm::mat4 getDirectionalLightViewMatrix();
	glm::vec3 getDirectionalLightVector();

	// The ambient light for the entire scene, to be added to directional and point light
	// contributions. Seems like a good rule of thumb may be to have ambient and
	// directional light add to no more than (1.0, 1.0, 1.0), so color is not washed out
	glm::vec3 getAmbientLightColor();
	void setAmbientLightColor (glm::vec3 color);

	// The preview light is used to show a preview when adding a new light, enabling it
	// includes it in the render, updating it automatically enables it as well.
	void updatePreviewLight(const glm::vec3& color, Point3di& position, float strength);
	void setPreviewLightEnabled(bool enabled);

	// This limit exists because GLSL for loops need to know how many iterations to perform
	// at compile time, so this can likely be changed somewhat arbitrarily as needed
	const static int MAX_NUM_LIGHTS = 16;

private:
	void rebuildDirectionalLightProperties();

	std::vector<Light*> lights;
	float directionalLightAngleX;
	float directionalLightAngleY;
	glm::vec3 directionalLightColor;
	bool directionalLightEnabled = true;
	glm::mat4 directionalLightViewMatrix;
	glm::vec3 directionalLightVector;
	glm::vec3 ambientLightColor;
	glm::vec3 renderPositionOffset;
	Light* selectedLight;

	Light* previewLight;
	bool previewLightEnabled;
};