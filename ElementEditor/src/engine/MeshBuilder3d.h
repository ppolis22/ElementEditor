#pragma once

#include "MeshBuilderBase.h"
#include "../vendor/glm/glm.hpp"

class MeshBuilder3d : public MeshBuilderBase {
public:
	MeshBuilder3d();
	~MeshBuilder3d();

	void addFace(Point3df& p0, Point3df& p1, Point3df& p2, Point3df& p3,	// four vertices of the face
		float p0Occ, float p1Occ, float p2Occ, float p3Occ,					// corresponding occlusion values per vertex
		Point3df& normal, glm::vec3 color);

protected:
	unsigned int buildVertexArray(unsigned int vertexBufferId, unsigned int indexBufferId) override;
};