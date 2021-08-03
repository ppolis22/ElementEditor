#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::render(Renderable& renderable) {
	// TODO get proj and view from instance of Camera class
	glm::mat4 m_Proj = glm::perspective(glm::radians(FOV), 960.0f / 540.0f, NEAR_PLANE, FAR_PLANE);
	glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 m_Model = renderable.getTransformation();
	glm::mat4 mvp = m_Proj * m_View * m_Model;

	Shader& chunkShader = renderable.getShader();
	chunkShader.bind();
	chunkShader.setUniformMat4f("u_MVP", mvp);

	Mesh& mesh = renderable.getMesh();
	glBindVertexArray(mesh.vertexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferId);
	glDrawElements(GL_TRIANGLES, mesh.indexBufferCount, GL_UNSIGNED_INT, nullptr);
}