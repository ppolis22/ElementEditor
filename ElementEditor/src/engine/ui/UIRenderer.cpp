#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "UIRenderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

UIRenderer::UIRenderer(MeshBuilder2d& meshBuilder) : shader("shaders/uiVertex.shader", "shaders/uiFragment.shader") {
	meshBuilder.addQuad({-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f});
	quad = meshBuilder.commitMesh();
}

UIRenderer::~UIRenderer() {
	MeshBuilder2d meshBuilder;		// TODO do this differently
	meshBuilder.deleteMesh(quad);
}

void UIRenderer::render(UIElement& element) {
	shader.bind();
	glm::mat4 transformationMatrix = buildTransformationMatrix(element);
	shader.setUniformMat4f("transformationMatrix", transformationMatrix);
	shader.setUniformVec4f("uiColor", glm::vec4(element.getColor(), element.getAlpha()));

	glBindVertexArray(quad.vertexArrayId);
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, quad.indexBufferCount, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader.unbind();
}

glm::mat4 UIRenderer::buildTransformationMatrix(UIElement& element) {
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(element.getPosition(), 0.0f));
	transform = glm::scale(transform, glm::vec3(element.getScale(), 1.0f));
	return transform;
}