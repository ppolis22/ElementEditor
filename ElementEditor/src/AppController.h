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

#include <vector>

class AppController {
public:
	AppController(BaseEditorState* state, Camera* camera, ModelRenderer* modelRenderer, ChunkManager* chunkManager, 
		UIRenderer* uiRenderer, Window* window);

	void addUIController(UIController* uiController);
	void setAddTool();
	void setSubtractTool();
	void processMouseMovement(MouseMoveEvent& event);
	void processScroll(MouseScrollEvent& event);
	void processKeyPress(KeyPressEvent& event);
	void processClick(MouseButtonUpEvent& event);
	void render();

	Camera* getCamera();
	ModelRenderer* getModelRenderer();
	ChunkManager* getChunkManager();
	Window* getWindow();

private:
	std::vector<UIController*> uiControllers;
	BaseEditorState* state;
	Camera* camera;
	ModelRenderer* modelRenderer;
	ChunkManager* chunkManager;
	Window* window;
	UIRenderer* uiRenderer;

	void changeActiveTool(BaseEditorState* newState);
};