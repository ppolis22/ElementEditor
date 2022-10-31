#pragma once

#include "Mesh.h"
#include "engine/geom/Point3d.h"

#include <vector>

class MeshBuilderBase {
public:
	MeshBuilderBase();
	virtual ~MeshBuilderBase();

	void createNewMesh();
	Mesh commitMesh();

protected:
	std::vector<float> currentVertexData;
	std::vector<unsigned int> currentIndices;
	unsigned int numVertices;

	virtual unsigned int buildVertexArray(unsigned int vertexBufferId, unsigned int indexBufferId) = 0;
	unsigned int buildIndexBuffer(unsigned int size, const unsigned int* data);
	unsigned int buildVertexBuffer(unsigned int size, const void* data);
};