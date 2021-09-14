#include "TranslateHandles.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

TranslateHandles::TranslateHandles()
	: mesh(buildMesh()),
	shader("shaders/handleVertex.shader", "shaders/handleFragment.shader"){}

Mesh& TranslateHandles::getMesh() {
	return mesh;
}

Shader& TranslateHandles::getShader() {
	return shader;
}

glm::mat4 TranslateHandles::getTransformation() {
	return glm::translate(glm::mat4(1.0f), position);
}

void TranslateHandles:: setPosition(glm::vec3 position) {
	this->position = position;
}

void TranslateHandles::setSelectedDirection(Direction direction) {
	// TODO highlight/rebuild mesh?
}

AABB TranslateHandles::getXBoundingBox() {
	return AABB{ 
		Point3df{ 0.0f + position.x, -0.2f + position.y, -0.2f + position.z }, 
		Point3df{ 1.0f + position.x, 0.2f + position.y, 0.2f + position.z } 
	};
}

Mesh TranslateHandles::buildMesh() {
	MeshBuilder3d meshBuilder;
	meshBuilder.createNewMesh();

	float leftX = 0.0f;
	float rightX = 1.0f;
	float downY = -0.2f;
	float upY = 0.2f;
	float nearZ = 0.2f;
	float farZ = -0.2f;

	Point3df leftBottomNear{ leftX, downY, nearZ };
	Point3df rightBottomNear{ rightX, downY, nearZ };
	Point3df leftTopNear{ leftX, upY, nearZ };
	Point3df rightTopNear{ rightX, upY, nearZ };
	Point3df leftBottomFar{ leftX, downY, farZ };
	Point3df rightBottomFar{ rightX, downY, farZ };
	Point3df leftTopFar{ leftX, upY, farZ };
	Point3df rightTopFar{ rightX, upY, farZ };

	Point3df backNormal{ 0.0f, 0.0f, -1.0f };
	meshBuilder.addFace(rightBottomFar, leftBottomFar, leftTopFar, rightTopFar, backNormal, Grass);

	Point3df frontNormal{ 0.0f, 0.0f, 1.0f };
	meshBuilder.addFace(leftBottomNear, rightBottomNear, rightTopNear, leftTopNear, frontNormal, Grass);

	Point3df bottomNormal{ 0.0f, -1.0f, 0.0f };
	meshBuilder.addFace(leftBottomFar, rightBottomFar, rightBottomNear, leftBottomNear, bottomNormal, Grass);

	Point3df topNormal{ 0.0f, 1.0f, 0.0f };
	meshBuilder.addFace(leftTopNear, rightTopNear, rightTopFar, leftTopFar, topNormal, Grass);

	return meshBuilder.commitMesh();
}