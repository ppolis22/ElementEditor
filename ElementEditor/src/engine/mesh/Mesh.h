#pragma once

struct Mesh {
	unsigned int vertexArrayId;
	unsigned int vertexBufferId;
	unsigned int indexBufferId;
	unsigned int indexBufferCount;

	void deleteMesh();
};