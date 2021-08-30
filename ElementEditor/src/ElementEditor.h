#pragma once

#include "engine/RenderedApplication.h"
#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"

class ElementEditor : public RenderedApplication {
public:
	ElementEditor();
	~ElementEditor();

	void processMouseMovement(float rawX, float rawY, float deltaX, float deltaY) override;
	void processKeyPress(int keyCode) override;
	void processScroll(float deltaY) override;
	void processMouseDown(int buttonCode, float posX, float posY) override;

protected:
	void run() override;

private:
	Camera camera;
	ModelRenderer renderer;
	ChunkManager chunkManager;

	void button1Pressed();
};