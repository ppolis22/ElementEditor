#pragma once

#include "LightManager.h"
#include "ChunkManager.h"
#include "editor/model/ProjectBounds.h"
#include "editor/file/ModelFileWriter.h"
#include "editor/state/BaseEditorState.h"

#include "engine/render/Camera.h"
#include "engine/render/ModelRenderer.h"
#include "engine/ui/UIRenderer.h"
#include "engine/event/EventListener.h"
#include "engine/event/MouseEvent.h"
#include "engine/event/KeyEvent.h"
#include "engine/app/Window.h"
#include "engine/geom/Point3d.h"
#include "engine/light/ShadowMapBuilder.h"

#include <vector>

class AppController : public EventListener {
public:
	AppController(Camera* camera, ModelRenderer* modelRenderer, ChunkManager* modelhunkManager, 
		ChunkManager* previewChunkManager, LightManager* lightManager, UIRenderer* uiRenderer, Window* window, 
		ProjectBounds* projectBounds, ModelFileWriter* fileWriter, ShadowMapBuilder* shadowMapBuilder);

	void initialize();

	void setState(State stateToSet);
	State getState();

	bool canSetMoveTool();
	bool canSetExtrudeTool();
	bool canSetAddLightTool();
	void setCanEditLights(bool canEdit);
	bool getCanEditLights();

	void saveProject();

	void processMouseMovement(MouseMoveEvent& event) override;
	void processScroll(MouseScrollEvent& event) override;
	void processKeyPress(KeyPressEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseDown(MouseButtonDownEvent& event) override;
	
	void render();
	BlockColor getActiveColor();
	void setActiveColor(BlockColor newColor);
	void addLight(Point3di position);
	void rebuildShadowMap();

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
	ModelFileWriter* fileWriter;
	ShadowMapBuilder* shadowMapBuilder;

	BlockColor activeColor;
	bool canEditLights = true;

	void changeActiveTool(BaseEditorState* newState);
};