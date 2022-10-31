#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "MeshBuilderTextured2d.h"

void MeshBuilderTextured2d::addQuad(const Point2df& p0, const Point2df& t0, const Point2df& p1, const Point2df& t1,
	const Point2df& p2, const Point2df& t2, const Point2df& p3, const Point2df& t3) {
	currentVertexData.insert(currentVertexData.end(), { p0.x, p0.y, t0.x, t0.y });		// x, y, u, v
	currentVertexData.insert(currentVertexData.end(), { p1.x, p1.y, t1.x, t1.y });
	currentVertexData.insert(currentVertexData.end(), { p2.x, p2.y, t2.x, t2.y });
	currentVertexData.insert(currentVertexData.end(), { p3.x, p3.y, t3.x, t3.y });

	currentIndices.insert(currentIndices.end(), { numVertices, numVertices + 1, numVertices + 2,
			numVertices + 2, numVertices + 3, numVertices });

	numVertices += 4;
}

unsigned int MeshBuilderTextured2d::buildVertexArray(unsigned int vertexBufferId, unsigned int indexBufferId) {
	unsigned int vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);		// index, size, type, normalized, stride, offset

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return vertexArrayId;
}