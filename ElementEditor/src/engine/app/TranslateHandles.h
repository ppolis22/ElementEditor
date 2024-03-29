#pragma once

#include "engine/render/Renderable.h"
#include "engine/mesh/MeshBuilder3d.h"
#include "engine/geom/AABB.h"

enum Direction {	// TODO find a better home for this
	X,
	Y,
	Z,
	NONE
};

class TranslateHandles : public Renderable {
public:
	TranslateHandles();
	~TranslateHandles();

	Mesh& getMesh();
	Shader& getShader();
	glm::mat4 getTransformation();

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	void setSelectedDirection(Direction direction);
	AABB getXBoundingBox();
	AABB getYBoundingBox();
	AABB getZBoundingBox();
	glm::vec3 getAxisVector(Direction direction);

private:
	Mesh mesh;
	Shader shader;
	glm::vec3 position;

	float xHandleLeft, xHandleRight, xHandleTop, xHandleBottom, xHandleNear, xHandleFar;
	float yHandleLeft, yHandleRight, yHandleTop, yHandleBottom, yHandleNear, yHandleFar;
	float zHandleLeft, zHandleRight, zHandleTop, zHandleBottom, zHandleNear, zHandleFar;
	glm::vec3 xColor, yColor, zColor;

	Mesh buildMesh();
	void buildHandleMesh(MeshBuilder3d& meshBuilder, glm::vec3 color, float left, float right, float top, float bottom, float near, float far);
};