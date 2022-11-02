#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "ShadowMapBuilder.h"

const unsigned int ShadowMapBuilder::SHADOWMAP_WIDTH = 1024;
const unsigned int ShadowMapBuilder::SHADOWMAP_HEIGHT = 1024;

ShadowMapBuilder::ShadowMapBuilder(DirectionalLight* directionalLight)
	: shadowMapShader("shaders/shadowMapVertex.shader", "shaders/shadowMapFragment.shader"),
	directionalLight(*directionalLight)
{
	glGenFramebuffers(1, &shadowMapFBO);

	glGenTextures(1, &shadowMapTextureId);
	glBindTexture(GL_TEXTURE_2D, shadowMapTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTextureId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int ShadowMapBuilder::getShadowMapTextureId() {
	return shadowMapTextureId;
}

static void renderMesh(Mesh& mesh) {
	glBindVertexArray(mesh.vertexArrayId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glDrawElements(GL_TRIANGLES, mesh.indexBufferCount, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glBindVertexArray(0);
}

void ShadowMapBuilder::buildShadowMap(std::vector<Renderable*> renderables) {
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 directionalLightProjectionMatrix = directionalLight.getProjectionMatrix();
	glm::mat4 directionalLightViewMatrix = directionalLight.getViewMatrix();

	shadowMapShader.bind();
	glm::mat4 directionalLightSpaceMatrix = directionalLightProjectionMatrix * directionalLightViewMatrix;
	shadowMapShader.setUniformMat4f("directionalLightSpaceMatrix", directionalLightSpaceMatrix);

	for (Renderable* renderable : renderables) {
		Mesh& mesh = renderable->getMesh();
		glm::mat4 modelMatrix = renderable->getTransformation();
		shadowMapShader.setUniformMat4f("modelMatrix", modelMatrix);

		renderMesh(mesh);
	}

	shadowMapShader.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}