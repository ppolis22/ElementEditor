#pragma once

#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"
#include "engine/ui/UIRenderer.h"
#include "state/BaseEditorState.h"
#include "engine/EventListener.h"
#include "engine/event/MouseEvent.h"
#include "engine/event/KeyEvent.h"
#include "engine/Window.h"
#include "engine/Point3d.h"

#include <vector>

class AppController : public EventListener {
public:
	AppController(Camera* camera, ModelRenderer* modelRenderer, ChunkManager* modelhunkManager, ChunkManager* previewChunkManager,
		UIRenderer* uiRenderer, Window* window);

	void setState(State stateToSet);
	State getState();

	bool canSetMoveTool();
	bool canSetExtrudeTool();

	void processMouseMovement(MouseMoveEvent& event) override;
	void processScroll(MouseScrollEvent& event) override;
	void processKeyPress(KeyPressEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseDown(MouseButtonDownEvent& event) override;
	
	void render();
	BlockColor getActiveColor();
	void setActiveColor(BlockColor newColor);

	Camera* getCamera();
	ModelRenderer* getModelRenderer();
	ChunkManager* getModelChunkManager();
	ChunkManager* getPreviewChunkManager();
	Window* getWindow();

private:
	BaseEditorState* state;
	Camera* camera;
	ModelRenderer* modelRenderer;
	ChunkManager* modelChunkManager;
	ChunkManager* previewChunkManager;
	Window* window;
	UIRenderer* uiRenderer;
	BlockColor activeColor;

	void changeActiveTool(BaseEditorState* newState);
};