#pragma once

#include "UIElement.h"
#include <functional>

class RenderRegion : public UIElement {
public:
	RenderRegion(float x, float y, float width, float height, std::function<void(MouseButtonUpEvent& e)> onMouseUp, 
		std::function<void(MouseButtonDownEvent& e)> onMouseDown, std::function<void(MouseScrollEvent& e)> onScroll, 
		std::function<void(MouseMoveEvent& e)> onMouseMove, std::function<void(KeyPressEvent& e)> onKeyPress);
	~RenderRegion();

	void processMouseMovement(MouseMoveEvent& event) override;
	void processKeyPress(KeyPressEvent& event) override;
	void processScroll(MouseScrollEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseDown(MouseButtonDownEvent& event) override;

private:
	std::function<void(MouseButtonUpEvent& e)> onMouseUp;
	std::function<void(MouseButtonDownEvent& e)> onMouseDown;
	std::function<void(MouseScrollEvent& e)> onScroll;
	std::function<void(KeyPressEvent& e)> onKeyPress;
	std::function<void(MouseMoveEvent& e)> onMouseMove;
};