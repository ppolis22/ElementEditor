#pragma once

class MeshBuilder {
public:
	MeshBuilder();
	~MeshBuilder();

	unsigned int createNewMesh();
	void deleteMesh();
	void addVertex();
};