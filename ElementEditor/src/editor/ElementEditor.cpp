#include "ElementEditor.h"

#include "editor/controller/AppController.h"
#include "editor/ui/ToolbarUI.h"
#include "editor/controller/LightManager.h"
#include "editor/file/ModelFileLoader.h"

#include "engine/app/Window.h"
#include "engine/render/ModelRenderer.h"
#include "engine/ui/UIRenderer.h"
#include "engine/ui/BasicUIRenderer.h"
#include "engine/light/DirectionalLight.h"
#include "engine/light/ShadowMapBuilder.h"

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

	ChunkManager* modelChunkManager = new ChunkManager();
	LightManager* lightManager = new LightManager();
	ProjectBounds* projectBounds = new ProjectBounds();
	Camera* camera = new Camera();

	ModelFileLoader loader(*modelChunkManager, *lightManager, *projectBounds, *camera);
	loader.load();

	MeshBuilder2d meshBuilder2d;
	MeshBuilderTextured2d meshBuilderTextured2d;
	UIRenderer* uiRenderer = new BasicUIRenderer(meshBuilder2d, meshBuilderTextured2d, window->getWidth(), window->getHeight());
	ChunkManager* previewChunkManager = new ChunkManager();
	ModelFileWriter* fileWriter = new ModelFileWriter(*modelChunkManager, *lightManager, *projectBounds, *camera);
	ShadowMapBuilder* shadowMapBuilder = new ShadowMapBuilder(lightManager->getDirectionalLight());
	ModelRenderer* modelRenderer = new ModelRenderer(window->getWidth(), window->getHeight(), shadowMapBuilder->getShadowMapTextureId());

	AppController appController(camera, modelRenderer, modelChunkManager, previewChunkManager, 
		lightManager, uiRenderer, window, projectBounds, fileWriter, shadowMapBuilder);
	window->setApplicationEventListener(&appController);

	ToolbarUI toolbarUI(&appController);
	window->setRootUIElement(&toolbarUI);

	appController.initialize();

	while (window->isOpen()) {
		glClearColor(0.85f, 0.90f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		appController.render();
		window->update();
	}
}