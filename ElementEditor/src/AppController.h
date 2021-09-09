#pragma once

#include "engine/ui/UIController.h"
#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"
#include "engine/ui/UIRenderer.h"
#include "BaseEditorState.h"
#include "engine/EventListener.h"
#include "engine/event/MouseEvent.h"
#include "engine/event/KeyEvent.h"
#include "engine/Window.h"
#include "engine/Point3d.h"

#include <vector>

class AppController {
public:
	AppController(Camera* camera, ModelRenderer* modelRenderer, ChunkManager* modelhunkManager, ChunkManager* previewChunkManager,
		UIRenderer* uiRenderer, Window* window);

	void addUIController(UIController* uiController);
	void setAddTool();
	void setSubtractTool();
	void setSelectTool();
	std::vector<Point3di>* getSelection();

	void processMouseMovement(MouseMoveEvent& event);
	void processScroll(MouseScrollEvent& event);
	void processKeyPress(KeyPressEvent& event);
	void processClick(MouseButtonUpEvent& event);
	void render();

	Camera* getCamera();
	ModelRenderer* getModelRenderer();
	ChunkManager* getModelChunkManager();
	ChunkManager* getPreviewChunkManager();
	Window* getWindow();

private:
	std::vector<UIController*> uiControllers;
	BaseEditorState* state;
	Camera* camera;
	ModelRenderer* modelRenderer;
	ChunkManager* modelChunkManager;
	ChunkManager* previewChunkManager;
	Window* window;
	UIRenderer* uiRenderer;
	std::vector<Point3di> selection;

	void changeActiveTool(BaseEditorState* newState);
};