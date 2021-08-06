#pragma once

#include "engine/RenderedApplication.h"
#include "engine/Camera.h"
#include "engine/Renderer.h"

class ElementEditor : public RenderedApplication {
public:
	ElementEditor();
	~ElementEditor();

	void processMouseMovement(float deltaX, float deltaY) override;
	void processKeyPress(int keyCode) override;
	void processScroll(float deltaY) override;

protected:
	void run() override;

private:
	Camera camera;
	Renderer renderer;
};