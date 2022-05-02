#include "ElementEditor.h"
#include "AppController.h"
#include "engine/Window.h"
#include "engine/ModelRenderer.h"
#include "engine/Point3d.h"
#include "engine/ui/UIRenderer.h"
#include "engine/ui/Button.h"
#include "engine/ui/BasicUIRenderer.h"
#include "engine/event/Event.h"
#include "ToolbarUI.h"
#include "LightManager.h"

#include "../vendor/glm/glm.hpp"

#include <vector>
#include <algorithm>

int main(void) {
	ElementEditor app;
	app.start();
}

ElementEditor::ElementEditor() {}

ElementEditor::~ElementEditor() {}

void ElementEditor::run() {

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	MeshBuilder2d meshBuilder2d;
	MeshBuilderTextured2d meshBuilderTextured2d;
	UIRenderer* uiRenderer = new BasicUIRenderer(meshBuilder2d, meshBuilderTextured2d, window->getWidth(), window->getHeight());
	Camera* camera = new Camera();
	ModelRenderer* modelRenderer = new ModelRenderer(window->getWidth(), window->getHeight());
	ChunkManager* modelChunkManager = new ChunkManager();
	ChunkManager* previewChunkManager = new ChunkManager();
	LightManager* lightManager = new LightManager();

	// TODO replace with some sort of Loader class
	BlockColor defaultColor{ 255, 255, 255 };
	modelChunkManager->setBlockColor(defaultColor, { 0, 0, 0 });
	modelChunkManager->setBlockColor(defaultColor, { 1, 0, 0 });
	modelChunkManager->setBlockColor(defaultColor, { 0, 1, 0 });
	modelChunkManager->setBlockColor(defaultColor, { 0, 0, 1 });
	modelChunkManager->rebuildChunkMeshes();

	lightManager->setDirectionalLightColor({ 0.5f, 0.5f, 0.5f });
	lightManager->setDirectionalLightPosition({ -10.0f, 3.5f, 3.5f });
	lightManager->setAmbientLightColor({0.5f, 0.5f, 0.5f});
	Light light{ glm::vec3(1.0f, 0.8f, 0.3f), glm::vec3(2.5f, 3.5f, 5.5f), 10.0f };	// color, position, strength
	lightManager->addLight(&light);

	AppController appController(camera, modelRenderer, modelChunkManager, previewChunkManager, lightManager, uiRenderer, window);
	window->setApplicationEventListener(&appController);

	ToolbarUI toolbarUI(&appController);
	window->setRootUIElement(&toolbarUI);
	appController.setActiveColor(defaultColor);

	while (window->isOpen()) {
		glClearColor(0.1f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		appController.render();
		window->update();
	}
}