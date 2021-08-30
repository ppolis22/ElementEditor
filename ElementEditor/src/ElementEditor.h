#pragma once

#include "engine/RenderedApplication.h"
#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"
#include "EditorState.h"

class ElementEditor : public RenderedApplication {
public:
	ElementEditor();
	~ElementEditor();

	Camera* getCamera();
	ChunkManager* getChunkManager();
	ModelRenderer* getModelRenderer();

	void processMouseMovement(float rawX, float rawY, float deltaX, float deltaY) override;
	void processScroll(float deltaY) override;
	void processMouseDown(int buttonCode, float posX, float posY) override;

protected:
	void run() override;

private:
	EditorState* state;

	Camera camera;
	ModelRenderer modelRenderer;
	ChunkManager chunkManager;

	void changeEditorState(EditorState* newState);
};