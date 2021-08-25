#pragma once

#include "MeshBuilderBase.h"

class MeshBuilder2d : public MeshBuilderBase {
public:
	MeshBuilder2d();
	~MeshBuilder2d();

	void addQuad(const Point2df& p0, const Point2df& p1, const Point2df& p2, const Point2df& p3);	// TODO add color

protected:
	unsigned int buildVertexArray(unsigned int vertexBufferId, unsigned int indexBufferId) override;
};