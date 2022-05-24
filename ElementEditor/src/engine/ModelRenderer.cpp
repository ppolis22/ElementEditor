#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "ModelRenderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include <sstream>

ModelRenderer::ModelRenderer(unsigned int renderRegionWidth, unsigned int renderRegionHeight, unsigned int shadowMapTextureId)
	: lineShader("shaders/lineVertex.shader", "shaders/lineFragment.shader"),
	renderRegionWidth(renderRegionWidth), renderRegionHeight(renderRegionHeight),
	shadowMapTextureId(shadowMapTextureId) {}

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

void ModelRenderer::renderNoLighting(
	std::vector<Renderable*> renderables,
	Shader& meshShader,
	Camera& camera,
	float alpha
) {
	if (renderables.empty())
		return;

	glEnable(GL_DEPTH_TEST);
	glm::mat4 projectionMatrix = camera.getProjectionMatrix();
	glm::mat4 viewMatrix = camera.getViewMatrix();

	meshShader.bind();
	meshShader.setUniformMat4f("projectionMatrix", projectionMatrix);
	meshShader.setUniformMat4f("viewMatrix", viewMatrix);
	meshShader.setUniformFloat("alpha", alpha);

	for (Renderable* renderable : renderables) {
		Mesh& mesh = renderable->getMesh();
		glm::mat4 modelMatrix = renderable->getTransformation();
		meshShader.setUniformMat4f("modelMatrix", modelMatrix);
		renderMesh(mesh);
	}
	meshShader.unbind();
}

void ModelRenderer::renderNoShadows(
	std::vector<Renderable*> renderables, 
	std::vector<Light*> pointLights, 
	DirectionalLight* directionalLight,
	glm::vec3 ambientLightColor,
	Shader& meshShader, 
	Camera& camera, 
	float alpha
) {
	if (renderables.empty())
		return;

	glEnable(GL_DEPTH_TEST);
	glm::mat4 projectionMatrix = camera.getProjectionMatrix();
	glm::mat4 viewMatrix = camera.getViewMatrix();
	glm::vec3 toDirectionalLightVector = -(directionalLight->getDirectionVector());

	meshShader.bind();
	meshShader.setUniformMat4f("projectionMatrix", projectionMatrix);
	meshShader.setUniformMat4f("viewMatrix", viewMatrix);
	meshShader.setUniformVec3f("toDirectionalLightVector", toDirectionalLightVector);
	meshShader.setUniformVec3f("directionalLightColor", directionalLight->getColor());
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
	DirectionalLight* directionalLight,
	glm::vec3 ambientLightColor, 
	Shader& meshShader, 
	Camera& camera, 
	float alpha
) {
	if (renderables.empty())
		return;

	// reset viewport after potential shadowmap render
	glViewport(0, 0, renderRegionWidth, renderRegionHeight);

	glm::mat4 projectionMatrix = camera.getProjectionMatrix();
	glm::mat4 viewMatrix = camera.getViewMatrix();
	glm::vec3 toDirectionalLightVector = -(directionalLight->getDirectionVector());

	glm::mat4 directionalLightProjectionMatrix = directionalLight->getProjectionMatrix();
	glm::mat4 directionalLightViewMatrix = directionalLight->getViewMatrix();
	glm::mat4 directionalLightSpaceMatrix = directionalLightProjectionMatrix * directionalLightViewMatrix;

	meshShader.bind();
	meshShader.setUniformMat4f("projectionMatrix", projectionMatrix);
	meshShader.setUniformMat4f("viewMatrix", viewMatrix);
	meshShader.setUniformMat4f("directionalLightSpaceMatrix", directionalLightSpaceMatrix);
	meshShader.setUniformVec3f("toDirectionalLightVector", toDirectionalLightVector);
	meshShader.setUniformVec3f("directionalLightColor", directionalLight->getColor());
	meshShader.setUniformVec3f("ambientLightColor", ambientLightColor);
	meshShader.setUniformFloat("alpha", alpha);

	meshShader.setUniform1i("numPointLights", pointLights.size());
	for (int i = 0; i < pointLights.size(); i++) {
		meshShader.setUniformVec3f(buildArrayUniformName("lights", i, "color"), pointLights[i]->getColor());
		meshShader.setUniformVec3f(buildArrayUniformName("lights", i, "position"), pointLights[i]->getRenderPosition());
		meshShader.setUniformFloat(buildArrayUniformName("lights", i, "strength"), pointLights[i]->getStrength());
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMapTextureId);
	meshShader.setUniform1i("shadowMap", 0);

	glEnable(GL_DEPTH_TEST);
	for (Renderable* renderable : renderables) {
		Mesh& mesh = renderable->getMesh();
		glm::mat4 modelMatrix = renderable->getTransformation();
		meshShader.setUniformMat4f("modelMatrix", modelMatrix);
		renderMesh(mesh);
	}
	meshShader.unbind();
}

void ModelRenderer::renderLines(Mesh& mesh, Camera& camera, glm::vec3 color) {
	lineShader.bind();
	lineShader.setUniformVec3f("lineColor", color);
	lineShader.setUniformMat4f("projectionMatrix", camera.getProjectionMatrix());
	lineShader.setUniformMat4f("viewMatrix", camera.getViewMatrix());
	lineShader.setUniformMat4f("modelMatrix", glm::mat4(1.0f));

	glBindVertexArray(mesh.vertexArrayId);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, mesh.indexBufferCount);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	lineShader.unbind();
}