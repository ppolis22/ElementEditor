#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "UIRenderer.h"

UIRenderer::UIRenderer(MeshBuilder2d& meshBuilder) : shader("shaders/uiVertex.shader", "shaders/uiFragment.shader") {
	meshBuilder.addQuad({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f});
	quad = meshBuilder.commitMesh();
}

UIRenderer::~UIRenderer() {
	MeshBuilder2d meshBuilder;		// TODO do this differently
	meshBuilder.deleteMesh(quad);
}

void UIRenderer::render(UIElement& element) {
	shader.bind();
	//shader.setUniformMat4f();

	glBindVertexArray(quad.vertexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.indexBufferId);		// can remove this line?
	glDrawElements(GL_TRIANGLES, quad.indexBufferCount, GL_UNSIGNED_INT, nullptr);
}