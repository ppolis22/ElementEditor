#pragma once

#include "MeshBuilderBase.h"
#include "../vendor/glm/glm.hpp"

class MeshBuilder3d : public MeshBuilderBase {
public:
	MeshBuilder3d();
	~MeshBuilder3d();

	void addFace(Point3df& p0, Point3df& p1, Point3df& p2, Point3df& p3, Point3df& normal, glm::vec3 color);

protected:
	unsigned int buildVertexArray(unsigned int vertexBufferId, unsigned int indexBufferId) override;
};