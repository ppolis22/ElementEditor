#pragma once

#include "MeshBuilderBase.h"

class MeshBuilderTextured2d : public MeshBuilderBase {
public:
	void addQuad(const Point2df& p0, const Point2df& t0, const Point2df& p1, const Point2df& t1,
		const Point2df& p2, const Point2df& t2, const Point2df& p3, const Point2df& t3);

protected:
	unsigned int buildVertexArray(unsigned int vertexBufferId, unsigned int indexBufferId) override;
};