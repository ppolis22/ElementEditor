#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::renderChunk(Chunk& chunk) {
	// need to obtain the x, y, and zPosition from chunk to create model transformation matrix
	glm::mat4 m_Proj = glm::perspective(glm::radians(FOV), 960.0f / 540.0f, NEAR_PLANE, FAR_PLANE);
	glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 m_Model = glm::rotate(glm::mat4(1.0f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mvp = m_Proj * m_View * m_Model;

	Shader& chunkShader = chunk.getShader();
	chunkShader.bind();
	chunkShader.setUniformMat4f("u_MVP", mvp);

	Mesh& mesh = chunk.getMesh();
	glBindVertexArray(mesh.vertexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferId);
	glDrawElements(GL_TRIANGLES, mesh.indexBufferCount, GL_UNSIGNED_INT, nullptr);
}