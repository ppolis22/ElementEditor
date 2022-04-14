#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "BasicUIRenderer.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../Texture.h"

BasicUIRenderer::BasicUIRenderer(MeshBuilder2d& meshBuilder, MeshBuilderTextured2d& texturedMeshBuidler, int windowWidth, int windowHeight)
	: coloredShader("shaders/uiVertex.shader", "shaders/uiFragment.shader"), 
	texturedShader("shaders/uiTexturedVertex.shader", "shaders/uiTexturedFragment.shader"),
	windowWidth(windowWidth), windowHeight(windowHeight) 
{
	meshBuilder.addQuad({ -1.0f, -1.0f }, { 1.0f, -1.0f }, { 1.0f, 1.0f }, { -1.0f, 1.0f });	// x, y, u, v
	coloredQuad = meshBuilder.commitMesh();

	texturedMeshBuidler.addQuad(
		{ -1.0f, -1.0f }, {0.0f, 0.0f},  // x, y, u, v
		{ 1.0f, -1.0f }, { 1.0f, 0.0f },
		{ 1.0f, 1.0f }, { 1.0f, 1.0f },
		{ -1.0f, 1.0f }, { 0.0f, 1.0f } 
	);
	texturedQuad = texturedMeshBuidler.commitMesh();
}

BasicUIRenderer::~BasicUIRenderer() {
	MeshBuilder2d meshBuilder;		// TODO do this differently
	meshBuilder.deleteMesh(coloredQuad);
	meshBuilder.deleteMesh(texturedQuad);
}

void BasicUIRenderer::renderColoredQuad(float x, float y, float width, float height, glm::vec3 color, float alpha) {
	coloredShader.bind();
	glm::mat4 transformationMatrix = buildTransformationMatrix(x, y, width, height);

	coloredShader.setUniformMat4f("transformationMatrix", transformationMatrix);
	coloredShader.setUniformVec4f("uiColor", glm::vec4(color, alpha));

	glBindVertexArray(coloredQuad.vertexArrayId);
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, coloredQuad.indexBufferCount, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	coloredShader.unbind();
}

void BasicUIRenderer::renderTexturedQuad(float x, float y, float width, float height, const std::string& texturePath) {
	texturedShader.bind();
	glm::mat4 transformationMatrix = buildTransformationMatrix(x, y, width, height);
	int textureSlot = getTextureSlot(texturePath);

	texturedShader.setUniformMat4f("transformationMatrix", transformationMatrix);
	texturedShader.setUniform1i("u_Texture", textureSlot);	// sets up sampler2D object called u_Texture, sampling from set texture slot

	glBindVertexArray(texturedQuad.vertexArrayId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, texturedQuad.indexBufferCount, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	texturedShader.unbind();
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

int BasicUIRenderer::getTextureSlot(const std::string& texturePath) {
	for (int slot = 0; slot < textures.size(); slot++) {
		if (textures[slot]->getPath() == texturePath) {
			return slot;
		}
	}

	Texture* newTexture = new Texture(texturePath);
	newTexture->bind(nextSlot);

	if (textures.size() == nextSlot) {
		textures.push_back(newTexture);
	} else if (textures[nextSlot] != nullptr) {
		delete textures[nextSlot];
		textures[nextSlot] = newTexture;
	}

	nextSlot = (nextSlot + 1) % TEX_SLOT_COUNT;
}