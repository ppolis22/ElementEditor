#pragma once

#include "MeshBuilderBase.h"

class MeshBuilder3d : public MeshBuilderBase {
public:
	MeshBuilder3d();
	~MeshBuilder3d();

	// replace BlockType param with like Color enum defined in engine, provide mapping in client
	void addFace(Point3df& p0, Point3df& p1, Point3df& p2, Point3df& p3, Point3df& normal, BlockType type);

protected:
	unsigned int buildVertexArray(unsigned int vertexBufferId, unsigned int indexBufferId) override;
};