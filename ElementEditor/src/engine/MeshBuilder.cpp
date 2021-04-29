#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "MeshBuilder.h"
#include "Mesh.h"

MeshBuilder::MeshBuilder() {}

MeshBuilder::~MeshBuilder() {}

void MeshBuilder::createNewMesh() {
	currentVertices.clear();
	currentIndices.clear();
	numVertices = 0;
}

Mesh MeshBuilder::commitMesh() {
	float* meshVertices = &currentVertices[0];
	unsigned int* meshIndices = &currentIndices[0];

	// Note: as of OpenGL 4.5, vertex buffers can be deleted after vertex array is made with it and the vao is then unbound
	unsigned int vertexBufferId = buildVertexBuffer(currentVertices.size() * sizeof(float), meshVertices);
	unsigned int indexBufferId = buildIndexBuffer(currentIndices.size() * sizeof(unsigned int), meshIndices);
	unsigned int vertexArrayId = buildVertexArray(vertexBufferId);

	return { vertexArrayId, vertexBufferId, indexBufferId };
}

void MeshBuilder::deleteMesh(Mesh& mesh) {
	// Probably should be done elsewhere?
	// Should this set all the ids in the input mesh to 0?
	glDeleteBuffers(1, &mesh.vertexBufferId);
	glDeleteBuffers(1, &mesh.indexBufferId);
	glDeleteVertexArrays(1, &mesh.vertexArrayId);
}

void MeshBuilder::addFace(Point3df& p0, Point3df& p1, Point3df& p2, Point3df& p3,
	Point3df& normal, BlockType type) {
	currentVertices.insert(currentVertices.end(), { p0.x, p0.y, p0.z, normal.x, normal.y, normal.z, (float)type });
	currentVertices.insert(currentVertices.end(), { p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, (float)type });
	currentVertices.insert(currentVertices.end(), { p2.x, p2.y, p2.z, normal.x, normal.y, normal.z, (float)type });
	currentVertices.insert(currentVertices.end(), { p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, (float)type });

	currentIndices.insert(currentIndices.end(), { numVertices, numVertices + 1, numVertices + 2, 
		numVertices + 2, numVertices + 3, numVertices});

	numVertices += 6;
}

unsigned int MeshBuilder::buildVertexBuffer(unsigned int size, const void* data) {
	unsigned int id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, (const void*)data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return id;
}

unsigned int MeshBuilder::buildIndexBuffer(unsigned int size, const unsigned int* data) {
	unsigned int id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return id;
}

unsigned int MeshBuilder::buildVertexArray(unsigned int vertexBufferId) {
	unsigned int vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)0);		// index, size, type, normalized, stride, offset
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*) (3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)(6 * sizeof(float)));
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vertexArrayId;
}