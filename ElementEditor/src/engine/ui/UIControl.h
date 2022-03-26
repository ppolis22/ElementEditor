#pragma once

#include "UIElement.h"
#include "../EventListener.h"
#include "../event/ActionListener.h"

class UIControl : public UIElement, public EventListener {
public:
	UIControl(float x, float y, float width, float height);
	~UIControl() {};

	virtual void processMouseMovement(MouseMoveEvent& event) override {};
	virtual void processKeyPress(KeyPressEvent& event) override {};
	virtual void processScroll(MouseScrollEvent& event) override {};
	virtual void processMouseDown(MouseButtonDownEvent& event) override {};
	virtual void processMouseUp(MouseButtonUpEvent& event) override {};

	virtual bool isEnabled();
	virtual void setEnabled(bool enabled);
	virtual void addListener(ActionListner* listener);

protected:
	bool isEnabled;
	std::vector<ActionListner*> listeners;

	virtual void alertListeners();
};