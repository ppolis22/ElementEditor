#pragma once

#include "../engine/EventListener.h"
#include "../engine/event/MouseEvent.h"
#include "../engine/event/KeyEvent.h"

class AppController;

	enum State {
		ADD,
		SUBTRACT,
		SELECT,
		MOVE,
		MOVE_LIGHT,
		EXTRUDE,
		COLOR_PICK,
		ERROR
	};

class BaseEditorState {
public:
	BaseEditorState(AppController* context);
	virtual ~BaseEditorState() {};

	virtual void processMouseMovement(MouseMoveEvent& event) {};
	virtual void processKeyPress(KeyPressEvent& event) {};
	virtual void processScroll(MouseScrollEvent& event) {};
	virtual void processMouseDown(MouseButtonDownEvent& event) {};
	virtual void processMouseUp(MouseButtonUpEvent& event) {};

	virtual State getType() = 0;
	virtual void render();
	virtual void init() {};
	virtual void cleanUp() {};

protected:
	AppController* context;
};