#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "MeshBuilder3d.h"

MeshBuilder3d::MeshBuilder3d() {}

MeshBuilder3d::~MeshBuilder3d() {}

void MeshBuilder3d::addFace(Point3df& p0, Point3df& p1, Point3df& p2, Point3df& p3,
	float p0Occ, float p1Occ, float p2Occ, float p3Occ, Point3df& normal, glm::vec3 color) {
	currentVertexData.insert(currentVertexData.end(), { p0.x, p0.y, p0.z, normal.x, normal.y, normal.z, color.r, color.g, color.b, p0Occ });
	currentVertexData.insert(currentVertexData.end(), { p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, color.r, color.g, color.b, p1Occ });
	currentVertexData.insert(currentVertexData.end(), { p2.x, p2.y, p2.z, normal.x, normal.y, normal.z, color.r, color.g, color.b, p2Occ });
	currentVertexData.insert(currentVertexData.end(), { p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, color.r, color.g, color.b, p3Occ });

	if (p0Occ + p2Occ > p1Occ + p3Occ) {
		currentIndices.insert(currentIndices.end(), { numVertices + 1, numVertices + 2, numVertices + 3,
		numVertices + 3, numVertices + 0, numVertices + 1});
	} else {
		currentIndices.insert(currentIndices.end(), { numVertices, numVertices + 1, numVertices + 2,
		numVertices + 2, numVertices + 3, numVertices });
	}

	numVertices += 4;
}

unsigned int MeshBuilder3d::buildVertexArray(unsigned int vertexBufferId, unsigned int indexBufferId) {
	unsigned int vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)0);		// index, size, type, normalized, stride, offset

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(9 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return vertexArrayId;
}