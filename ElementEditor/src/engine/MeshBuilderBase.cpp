#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "MeshBuilderBase.h"
#include "Mesh.h"

MeshBuilderBase::MeshBuilderBase() {}

MeshBuilderBase::~MeshBuilderBase() {}

void MeshBuilderBase::createNewMesh() {
	currentVertexData.clear();
	currentIndices.clear();
	numVertices = 0;
}

Mesh MeshBuilderBase::commitMesh() {
	if (currentVertexData.size() == 0 || currentIndices.size() == 0) {
		return { 0, 0, 0, 0 };
	}

	float* meshVertices = &currentVertexData[0];
	unsigned int* meshIndices = &currentIndices[0];

	// Note: as of OpenGL 4.5, vertex buffers can be deleted after vertex array is made with it and the vao is then unbound
	unsigned int vertexBufferId = buildVertexBuffer(currentVertexData.size() * sizeof(float), meshVertices);
	unsigned int indexBufferId = buildIndexBuffer(currentIndices.size() * sizeof(unsigned int), meshIndices);
	unsigned int vertexArrayId = buildVertexArray(vertexBufferId, indexBufferId);

	return { vertexArrayId, vertexBufferId, indexBufferId, currentIndices.size() };
}

void MeshBuilderBase::deleteMesh(Mesh& mesh) {
	// Probably should be done elsewhere?
	// Should this set all the ids in the input mesh to 0?
	glDeleteBuffers(1, &mesh.vertexBufferId);
	glDeleteBuffers(1, &mesh.indexBufferId);
	glDeleteVertexArrays(1, &mesh.vertexArrayId);
}

unsigned int MeshBuilderBase::buildVertexBuffer(unsigned int size, const void* data) {
	unsigned int id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, (const void*)data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return id;
}

unsigned int MeshBuilderBase::buildIndexBuffer(unsigned int size, const unsigned int* data) {
	unsigned int id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);	// if this is done while VAO is bound, then simply binding the VAO later also binds this IBO, TODO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return id;
}