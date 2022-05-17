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
#include "LightManager.h"
#include "ProjectBounds.h"

#include <vector>

class AppController : public EventListener {
public:
	AppController(Camera* camera, ModelRenderer* modelRenderer, ChunkManager* modelhunkManager, ChunkManager* previewChunkManager,
		LightManager* lightManager, UIRenderer* uiRenderer, Window* window, ProjectBounds* projectBounds);

	void initialize();

	void setState(State stateToSet);
	State getState();

	bool canSetMoveTool();
	bool canSetExtrudeTool();
	bool canSetAddLightTool();
	void setCanEditLights(bool canEdit);
	bool getCanEditLights();

	void processMouseMovement(MouseMoveEvent& event) override;
	void processScroll(MouseScrollEvent& event) override;
	void processKeyPress(KeyPressEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseDown(MouseButtonDownEvent& event) override;
	
	void render();
	BlockColor getActiveColor();
	void setActiveColor(BlockColor newColor);
	void addLight(Point3di position);

	Camera* getCamera();
	ModelRenderer* getModelRenderer();
	ChunkManager* getModelChunkManager();
	ChunkManager* getPreviewChunkManager();
	UIRenderer* getUIRenderer();
	LightManager* getLightManager();
	Window* getWindow();
	ProjectBounds* getProjectBounds();

private:
	BaseEditorState* state;
	Camera* camera;
	ModelRenderer* modelRenderer;
	ChunkManager* modelChunkManager;
	ChunkManager* previewChunkManager;
	LightManager* lightManager;
	Window* window;
	ProjectBounds* projectBounds;
	UIRenderer* uiRenderer;
	BlockColor activeColor;
	bool canEditLights = true;

	void changeActiveTool(BaseEditorState* newState);
};