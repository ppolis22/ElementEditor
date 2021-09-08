#pragma once

#include "engine/EventListener.h"
#include "engine/event/MouseEvent.h"
#include "engine/event/KeyEvent.h"

class AppController;

class BaseEditorState {
public:
	BaseEditorState(AppController* context);
	virtual ~BaseEditorState();

	virtual void processMouseMovement(MouseMoveEvent& event);
	virtual void processKeyPress(KeyPressEvent& event);
	virtual void processScroll(MouseScrollEvent& event);
	virtual void processClick(MouseButtonUpEvent& event);

	virtual void render();

protected:
	AppController* context;
};