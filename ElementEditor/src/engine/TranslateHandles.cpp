#include "TranslateHandles.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

TranslateHandles::TranslateHandles()
	: shader("shaders/handleVertex.shader", "shaders/handleFragment.shader") {
	xHandleLeft = 0.1f;
	xHandleRight = 1.0f;
	xHandleTop = 0.1f;
	xHandleBottom = -0.1f;
	xHandleNear = 0.1f;
	xHandleFar = -0.1f;

	yHandleLeft = -0.1f;
	yHandleRight = 0.1f;
	yHandleTop = 1.0f;
	yHandleBottom = 0.1f;
	yHandleNear = 0.1f;
	yHandleFar = -0.1f;

	zHandleLeft = -0.1f;
	zHandleRight = 0.1f;
	zHandleTop = 0.1f;
	zHandleBottom = -0.1f;
	zHandleNear = 1.0f;
	zHandleFar = 0.1f;

	mesh = buildMesh();
}

TranslateHandles::~TranslateHandles() {
	MeshBuilder3d meshBuilder;
	meshBuilder.deleteMesh(mesh);
}

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

glm::vec3 TranslateHandles::getPosition() {
	return position;
}

void TranslateHandles::setSelectedDirection(Direction direction) {
	// TODO highlight/rebuild mesh?
}

glm::vec3 TranslateHandles::getAxisVector(Direction direction) {
	if (direction == X) {
		return glm::vec3(1.0f, 0.0f, 0.0f);
	} else if (direction == Y) {
		return glm::vec3(0.0f, 1.0f, 0.0f);
	} else if (direction == Z) {
		return glm::vec3(0.0f, 0.0f, 1.0f);
	} else {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

AABB TranslateHandles::getXBoundingBox() {
	return AABB{ 
		Point3df{ xHandleLeft + position.x, xHandleBottom + position.y, xHandleFar + position.z }, 
		Point3df{ xHandleRight + position.x, xHandleTop + position.y, xHandleNear + position.z } 
	};
}

AABB TranslateHandles::getYBoundingBox() {
	return AABB{
		Point3df{ yHandleLeft + position.x, yHandleBottom + position.y, yHandleFar + position.z },
		Point3df{ yHandleRight + position.x, yHandleTop + position.y, yHandleNear + position.z }
	};
}

AABB TranslateHandles::getZBoundingBox() {
	return AABB{
		Point3df{ zHandleLeft + position.x, zHandleBottom + position.y, zHandleFar + position.z },
		Point3df{ zHandleRight + position.x, zHandleTop + position.y, zHandleNear + position.z }
	};
}

Mesh TranslateHandles::buildMesh() {
	MeshBuilder3d meshBuilder;
	meshBuilder.createNewMesh();
	
	buildHandleMesh(meshBuilder, Grass, xHandleLeft, xHandleRight, xHandleTop, xHandleBottom, xHandleNear, xHandleFar);
	buildHandleMesh(meshBuilder, Stone, yHandleLeft, yHandleRight, yHandleTop, yHandleBottom, yHandleNear, yHandleFar);
	buildHandleMesh(meshBuilder, Selected, zHandleLeft, zHandleRight, zHandleTop, zHandleBottom, zHandleNear, zHandleFar);

	return meshBuilder.commitMesh();
}

void TranslateHandles::buildHandleMesh(MeshBuilder3d& meshBuilder, BlockType color, 
	float left, float right, float top, float bottom, float near, float far) {
	Point3df leftBottomNear{ left, bottom, near };
	Point3df rightBottomNear{ right, bottom, near };
	Point3df leftTopNear{ left, top, near };
	Point3df rightTopNear{ right, top, near };
	Point3df leftBottomFar{ left, bottom, far };
	Point3df rightBottomFar{ right, bottom, far };
	Point3df leftTopFar{ left, top, far };
	Point3df rightTopFar{ right, top, far };

	Point3df leftNormal{ -1.0f, 0.0f, 0.0f };
	meshBuilder.addFace(leftBottomFar, leftBottomNear, leftTopNear, leftTopFar, leftNormal, color);

	Point3df rightNormal{ 1.0f, 0.0f, 0.0f };
	meshBuilder.addFace(rightBottomNear, rightBottomFar, rightTopFar, rightTopNear, rightNormal, color);

	Point3df backNormal{ 0.0f, 0.0f, -1.0f };
	meshBuilder.addFace(rightBottomFar, leftBottomFar, leftTopFar, rightTopFar, backNormal, color);

	Point3df frontNormal{ 0.0f, 0.0f, 1.0f };
	meshBuilder.addFace(leftBottomNear, rightBottomNear, rightTopNear, leftTopNear, frontNormal, color);

	Point3df bottomNormal{ 0.0f, -1.0f, 0.0f };
	meshBuilder.addFace(leftBottomFar, rightBottomFar, rightBottomNear, leftBottomNear, bottomNormal, color);

	Point3df topNormal{ 0.0f, 1.0f, 0.0f };
	meshBuilder.addFace(leftTopNear, rightTopNear, rightTopFar, leftTopFar, topNormal, color);
}