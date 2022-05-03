#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "ModelRenderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include <sstream>

ModelRenderer::ModelRenderer(unsigned int renderRegionWidth, unsigned int renderRegionHeight)
	: shadowMapShader("shaders/shadowMapVertex.shader", "shaders/shadowMapFragment.shader"),
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

static const std::string buildArrayUniformName(const char* arrayName, int index, const char* propertyName) {
	std::stringstream ss;
	ss << arrayName << "[" << index << "]." << propertyName;
	return ss.str();
}

void ModelRenderer::renderNoShadows(
	std::vector<Renderable*> renderables, 
	std::vector<Light*> pointLights, 
	glm::vec3 directionalLightColor, 
	glm::vec3 directionalLightPosition,
	glm::vec3 ambientLightColor,
	Shader& meshShader, 
	Camera& camera, 
	float alpha
) {
	if (renderables.empty()) {
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glm::mat4 projectionMatrix = camera.getProjectionMatrix();
	glm::mat4 viewMatrix = camera.getViewMatrix();

	meshShader.bind();
	meshShader.setUniformMat4f("projectionMatrix", projectionMatrix);
	meshShader.setUniformMat4f("viewMatrix", viewMatrix);
	meshShader.setUniformVec3f("directionalLightPosition", directionalLightPosition);
	meshShader.setUniformVec3f("directionalLightColor", directionalLightColor);
	meshShader.setUniformVec3f("ambientLightColor", ambientLightColor);
	meshShader.setUniformFloat("alpha", alpha);

	meshShader.setUniform1i("numPointLights", pointLights.size());
	for (int i = 0; i < pointLights.size(); i++) {
		meshShader.setUniformVec3f(buildArrayUniformName("lights", i, "color"), pointLights[i]->getColor());
		meshShader.setUniformVec3f(buildArrayUniformName("lights", i, "position"), pointLights[i]->getRenderPosition());
		meshShader.setUniformFloat(buildArrayUniformName("lights", i, "strength"), pointLights[i]->getStrength());
	}

	for (Renderable* renderable : renderables) {
		Mesh& mesh = renderable->getMesh();
		glm::mat4 modelMatrix = renderable->getTransformation();
		meshShader.setUniformMat4f("modelMatrix", modelMatrix);
		renderMesh(mesh);
	}
	meshShader.unbind();
}

void ModelRenderer::renderWithShadows(
	std::vector<Renderable*> renderables, 
	std::vector<Light*> pointLights,
	glm::vec3 directionalLightColor, 
	glm::vec3 directionalLightPosition, 
	glm::vec3 ambientLightColor, 
	Shader& meshShader, 
	Camera& camera, 
	float alpha
) {
	if (renderables.empty()) {
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glm::mat4 lightViewMatrix = glm::lookAt(
		directionalLightPosition,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	// render shadow map
	glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	float near_plane = 1.0f, far_plane = 25.0f;
	glm::mat4 lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	shadowMapShader.bind();
	glm::mat4 directionalLightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;
	shadowMapShader.setUniformMat4f("directionalLightSpaceMatrix", directionalLightSpaceMatrix);

	for (Renderable* renderable : renderables) {
		Mesh& mesh = renderable->getMesh();
		glm::mat4 modelMatrix = renderable->getTransformation();
		shadowMapShader.setUniformMat4f("modelMatrix", modelMatrix);

		renderMesh(mesh);
	}

	shadowMapShader.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render geometry
	glViewport(0, 0, renderRegionWidth, renderRegionHeight);

	glm::mat4 projectionMatrix = camera.getProjectionMatrix();
	glm::mat4 viewMatrix = camera.getViewMatrix();

	meshShader.bind();
	meshShader.setUniformMat4f("projectionMatrix", projectionMatrix);
	meshShader.setUniformMat4f("viewMatrix", viewMatrix);
	meshShader.setUniformMat4f("directionalLightSpaceMatrix", directionalLightSpaceMatrix);
	meshShader.setUniformVec3f("directionalLightPosition", directionalLightPosition);
	meshShader.setUniformVec3f("directionalLightColor", directionalLightColor);
	meshShader.setUniformVec3f("ambientLightColor", ambientLightColor);
	meshShader.setUniformFloat("alpha", alpha);

	meshShader.setUniform1i("numPointLights", pointLights.size());
	for (int i = 0; i < pointLights.size(); i++) {
		meshShader.setUniformVec3f(buildArrayUniformName("lights", i, "color"), pointLights[i]->getColor());
		meshShader.setUniformVec3f(buildArrayUniformName("lights", i, "position"), pointLights[i]->getRenderPosition());
		meshShader.setUniformFloat(buildArrayUniformName("lights", i, "strength"), pointLights[i]->getStrength());
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
	meshShader.setUniform1i("shadowMap", 0);

	for (Renderable* renderable : renderables) {
		Mesh& mesh = renderable->getMesh();
		glm::mat4 modelMatrix = renderable->getTransformation();
		meshShader.setUniformMat4f("modelMatrix", modelMatrix);
		renderMesh(mesh);
	}
	meshShader.unbind();
}