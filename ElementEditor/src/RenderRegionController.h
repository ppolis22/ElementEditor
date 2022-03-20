#pragma once

#include "UIController.h"
#include "engine/ui/RenderRegion.h"
#include "engine/Window.h"
#include "engine/event/KeyEvent.h"
#include "engine/event/MouseEvent.h"

class RenderRegionController : public UIController {
public:
	RenderRegionController(Window* window);
	void update() override;

private:
	RenderRegion renderRegion;

	void mouseUpInRenderRegion(MouseButtonUpEvent& event);
	void mouseDownInRenderRegion(MouseButtonDownEvent& event);
	void scrolled(MouseScrollEvent& event);
	void mouseMovedInRenderRegion(MouseMoveEvent& event);
	void keyPressed(KeyPressEvent& event);
};