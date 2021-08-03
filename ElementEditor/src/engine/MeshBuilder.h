#pragma once

#include "../Block.h"
#include "Point3d.h"
#include <vector>

class MeshBuilder {
public:
	MeshBuilder();
	~MeshBuilder();

	void createNewMesh();
	Mesh commitMesh();
	void deleteMesh(Mesh& mesh);

	void addFace(Point3df& p0, Point3df& p1, Point3df& p2, Point3df& p3,
		Point3df& normal, BlockType type);	// replace BlockType param with like Color enum defined in engine, provide mapping in client

private:
	std::vector<float> currentVertexData;
	std::vector<unsigned int> currentIndices;
	unsigned int numVertices;

	unsigned int buildVertexArray(unsigned int vertexBufferId);
	unsigned int buildIndexBuffer(unsigned int size, const unsigned int* data);
	unsigned int buildVertexBuffer(unsigned int size, const void* data);
};