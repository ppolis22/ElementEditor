#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "Mesh.h"

void Mesh::deleteMesh() {
	// Probably should be done elsewhere?
	// Should this set all the ids in the input mesh to 0?
	glDeleteBuffers(1, &vertexBufferId);
	glDeleteBuffers(1, &indexBufferId);
	glDeleteVertexArrays(1, &vertexArrayId);
}