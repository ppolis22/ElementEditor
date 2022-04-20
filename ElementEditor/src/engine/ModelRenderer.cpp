#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "ModelRenderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

ModelRenderer::ModelRenderer(unsigned int renderRegionWidth, unsigned int renderRegionHeight)
	: shadowMapShader("shaders/shadowMapVertex.shader", "shaders/shadowMapFragment.shader"),
	debugQuadShader("shaders/debugQuadVertex.shader", "shaders/debugQuadFragment.shader"),
	renderRegionWidth(renderRegionWidth), renderRegionHeight(renderRegionHeight)
{
	glGenFramebuffers(1, &shadowMapFBO);

	glGenTextures(1, &depthMapTextureId);
	glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTextureId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ModelRenderer::~ModelRenderer() {}

void ModelRenderer::render(std::vector<Chunk>& chunks, Camera& camera) {
	renderWithTransparency(chunks, camera, 1.0f);
}

void ModelRenderer::renderPreview(std::vector<Chunk>& previewChunks, Camera& camera) {
	renderWithTransparency(previewChunks, camera, 0.5f);
}

void ModelRenderer::renderQuad() {
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

static void renderMesh(Mesh& mesh) {
	glBindVertexArray(mesh.vertexArrayId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, mesh.indexBufferCount, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void ModelRenderer::renderWithTransparency(std::vector<Chunk>& chunks, Camera& camera, float alpha) {

	if (chunks.empty()) {
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glm::vec3 lightPosition(-3.0f, 5.0f, 0.0f);		//TODO move to Light class
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::mat4 lightViewMatrix = glm::lookAt(
		lightPosition,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	// render shadow map
	glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	float near_plane = 1.0f, far_plane = 25.0f;
	glm::mat4 lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	shadowMapShader.bind();
	glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;
	shadowMapShader.setUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

	for (Chunk& chunk : chunks) {
		Mesh& mesh = chunk.getMesh();
		glm::mat4 modelMatrix = chunk.getTransformation();
		shadowMapShader.setUniformMat4f("modelMatrix", modelMatrix);

		renderMesh(mesh);
	}

	shadowMapShader.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render chunk geometry
	glViewport(0, 0, renderRegionWidth, renderRegionHeight);

	glm::mat4 projectionMatrix = camera.getProjectionMatrix();
	glm::mat4 viewMatrix = camera.getViewMatrix();

	Shader& chunkShader = chunks[0].getShader();		// TODO check size
	chunkShader.bind();
	chunkShader.setUniformMat4f("projectionMatrix", projectionMatrix);
	chunkShader.setUniformMat4f("viewMatrix", viewMatrix);
	chunkShader.setUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
	chunkShader.setUniformVec3f("lightPosition", lightPosition);
	chunkShader.setUniformVec3f("lightColor", lightColor);
	chunkShader.setUniformFloat("alpha", alpha);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
	chunkShader.setUniform1i("shadowMap", 0);

	for (Chunk& chunk : chunks) {
		Mesh& mesh = chunk.getMesh();
		glm::mat4 modelMatrix = chunk.getTransformation();
		chunkShader.setUniformMat4f("modelMatrix", modelMatrix);
		renderMesh(mesh);
	}
	chunkShader.unbind();

	// render debug quad
	/*debugQuadShader.bind();
	debugQuadShader.setUniformFloat("near_plane", near_plane);
	debugQuadShader.setUniformFloat("far_plane", far_plane);
	debugQuadShader.setUniform1i("depthMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
	renderQuad();*/
}