#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::render(Renderable& renderable, Camera& camera) {
	glm::mat4 projectionMatrix = camera.getProjectionMatrix();
	glm::mat4 viewMatrix = camera.getViewMatrix();
	glm::mat4 modelMatrix = renderable.getTransformation();

	glm::vec3 lightPosition(5.0f, 3.0f, 2.0f);		//TODO move to Light class
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	Shader& chunkShader = renderable.getShader();
	chunkShader.bind();
	chunkShader.setUniformMat4f("projectionMatrix", projectionMatrix);
	chunkShader.setUniformMat4f("viewMatrix", viewMatrix);
	chunkShader.setUniformMat4f("modelMatrix", modelMatrix);
	chunkShader.setUniformVec3f("lightPosition", lightPosition);
	chunkShader.setUniformVec3f("lightColor", lightColor);

	Mesh& mesh = renderable.getMesh();
	glBindVertexArray(mesh.vertexArrayId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, mesh.indexBufferCount, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	chunkShader.unbind();
}