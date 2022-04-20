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

	// TODO replace with some sort of Loader class
	BlockColor defaultColor{ 0, 0, 255 };
	modelChunkManager->setBlockColor(defaultColor, { 0, 0, 0 });
	modelChunkManager->setBlockColor(defaultColor, { 1, 0, 0 });
	modelChunkManager->setBlockColor(defaultColor, { 0, 1, 0 });
	modelChunkManager->setBlockColor(defaultColor, { 0, 0, 1 });
	modelChunkManager->rebuildChunkMeshes();

	AppController appController(camera, modelRenderer, modelChunkManager, previewChunkManager, uiRenderer, window);
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