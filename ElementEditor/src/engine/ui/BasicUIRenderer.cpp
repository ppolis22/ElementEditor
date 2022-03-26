#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "BasicUIRenderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

BasicUIRenderer::BasicUIRenderer(MeshBuilder2d& meshBuilder, int windowWidth, int windowHeight)
	: shader("shaders/uiVertex.shader", "shaders/uiFragment.shader"), windowWidth(windowWidth), windowHeight(windowHeight) {
	meshBuilder.addQuad({ -1.0f, -1.0f }, { 1.0f, -1.0f }, { 1.0f, 1.0f }, { -1.0f, 1.0f });
	quad = meshBuilder.commitMesh();
}

BasicUIRenderer::~BasicUIRenderer() {
	MeshBuilder2d meshBuilder;		// TODO do this differently
	meshBuilder.deleteMesh(quad);
}

void BasicUIRenderer::renderQuad(float x, float y, float width, float height, glm::vec3 color, float alpha) {
	shader.bind();
	glm::mat4 transformationMatrix = buildTransformationMatrix(x, y, width, height);
	shader.setUniformMat4f("transformationMatrix", transformationMatrix);
	shader.setUniformVec4f("uiColor", glm::vec4(color, alpha));

	glBindVertexArray(quad.vertexArrayId);
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, quad.indexBufferCount, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader.unbind();
}

glm::mat4 BasicUIRenderer::buildTransformationMatrix(float x, float y, float width, float height) {
	glm::mat4 transform(1.0f);
	float viewPortX = ((x + (width / 2.0f)) * 2.0f / (float)windowWidth) - 1.0f;
	float viewPortY = ((y + (height / 2.0f)) * -2.0f / (float)windowHeight) + 1.0f;
	transform = glm::translate(transform, glm::vec3(viewPortX, viewPortY, 0.0f));
	float widthRatio = width / (float)windowWidth;
	float heightRatio = height / (float)windowHeight;
	transform = glm::scale(transform, glm::vec3(widthRatio, heightRatio, 1.0f));
	return transform;
}