#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class DirectionalLight {
public:
	DirectionalLight(int xBound, int yBound, int zBound, float yAngle, float xAngle, glm::vec3 color);

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::vec3 getDirectionVector();

	glm::vec3 getColor();
	int getXBound() { return xBound;  }
	int getYBound() { return yBound;  }
	int getZBound() { return zBound;  }
	float getYAngle() { return yAngle; }
	float getXAngle() { return xAngle; }

private:
	struct ShadowMapCastData {
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 directionVector;
	} shadowMapCastData;

	ShadowMapCastData computeShadowMapCastData(float yAngle, float xAngle, int projectWidth, int projectDepth, int projectHeight);
	glm::vec3 color;
	float yAngle, xAngle;
	int xBound, yBound, zBound;
};