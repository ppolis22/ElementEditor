#include "RenderRegion.h"

RenderRegion::RenderRegion(float x, float y, float width, float height, std::function<void(MouseButtonUpEvent& e)> onClick,
	std::function<void(MouseScrollEvent& e)> onScroll, std::function<void(MouseMoveEvent& e)> onMouseMove,
	std::function<void(KeyPressEvent& e)> onKeyPress)
	: UIElement(x, y, width, height, glm::vec3(0.0, 0.0, 0.0), 1.0, true), 
	onClick(onClick), onScroll(onScroll), onMouseMove(onMouseMove), onKeyPress(onKeyPress) {}

RenderRegion::~RenderRegion() {}

void RenderRegion::processMouseMovement(MouseMoveEvent& event) {
	onMouseMove(event);
}

void RenderRegion::processKeyPress(KeyPressEvent& event) {
	onKeyPress(event);
}

void RenderRegion::processScroll(MouseScrollEvent& event) {
	onScroll(event);
}

void RenderRegion::processMouseUp(MouseButtonUpEvent& event) {
	onClick(event);
}