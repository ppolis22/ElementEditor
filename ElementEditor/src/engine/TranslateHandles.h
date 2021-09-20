#pragma once

#include "Renderable.h"
#include "MeshBuilder3d.h"
#include "AABB.h"

enum Direction {	// TODO find a better home for this
	X,
	Y,
	Z,
	NONE
};

class TranslateHandles : public Renderable {
public:
	TranslateHandles();

	Mesh& getMesh();
	Shader& getShader();
	glm::mat4 getTransformation();

	void setPosition(glm::vec3 position);
	void setSelectedDirection(Direction direction);
	AABB getXBoundingBox();

private:
	Mesh mesh;
	Shader shader;
	glm::vec3 position;

	float xHandleLeft, xHandleRight, xHandleTop, xHandleBottom, xHandleNear, xHandleFar;
	float yHandleLeft, yHandleRight, yHandleTop, yHandleBottom, yHandleNear, yHandleFar;
	float zHandleLeft, zHandleRight, zHandleTop, zHandleBottom, zHandleNear, zHandleFar;
	BlockType xColor, yColor, zColor;

	Mesh buildMesh();
	void buildHandleMesh(MeshBuilder3d& meshBuilder, BlockType color, float left, float right, float top, float bottom, float near, float far);
};