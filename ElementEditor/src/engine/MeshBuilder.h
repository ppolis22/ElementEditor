#pragma once

#include "../Block.h"
#include "Point3d.h"

class MeshBuilder {
public:
	MeshBuilder();
	~MeshBuilder();

	unsigned int createNewMesh();
	void commitMesh(unsigned int id);
	void deleteMesh(unsigned int id);

	void addFace(Point3df& p0, Point3df& p1, Point3df& p2, Point3df& p3,
		Point3df& normal, BlockType type);	// replace BlockType param with like Color enum defined in engine, provide mapping in client
};