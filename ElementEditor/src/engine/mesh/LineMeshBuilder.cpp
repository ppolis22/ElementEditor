#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "LineMeshBuilder.h"

#include <vector>

Mesh LineMeshBuilder::buildPlaneLineMesh(Point3df p0, Point3df p1, Point3df p2, Point3df p3) {
	unsigned int vao;
	unsigned int vbo;
	std::vector<float> vertices;

	// investigate using index buffer here to eliminate redundant data
	vertices.insert(vertices.end(), { p0.x, p0.y, p0.z });
	vertices.insert(vertices.end(), { p1.x, p1.y, p1.z });
	vertices.insert(vertices.end(), { p2.x, p2.y, p2.z });
	vertices.insert(vertices.end(), { p3.x, p3.y, p3.z });

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (const void*)vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// index, size, type, normalized, stride, offset

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// using indexBufferSize to store vbo size, should create own type
	Mesh mesh{ vao, vbo, 0, vertices.size() / 3 };
	return mesh;
}