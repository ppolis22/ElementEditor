#pragma once

#include "engine/event/EventListener.h"
#include "engine/event/MouseEvent.h"
#include "engine/event/KeyEvent.h"
#include "engine/geom/RayTracer.h"

class AppController;

	enum State {
		ADD,
		SUBTRACT,
		SELECT,
		MOVE,
		EXTRUDE,
		COLOR_PICK,
		ADD_LIGHT,
		EDIT_LIGHT,
		REMOVE_LIGHT,
		ERROR
	};

class BaseEditorState {
public:
	BaseEditorState(AppController* context);
	virtual ~BaseEditorState() {};

	virtual void processKeyPress(KeyPressEvent& event);
	virtual void processMouseMovement(MouseMoveEvent& event) {};
	virtual void processScroll(MouseScrollEvent& event) {};
	virtual void processMouseDown(MouseButtonDownEvent& event) {};
	virtual void processMouseUp(MouseButtonUpEvent& event) {};

	virtual State getType() = 0;
	virtual void init() {};
	virtual void cleanUp() {};
	virtual void render();

	void renderModelChunks();
	void renderPreviewChunks();
	void renderLightIcons();
	void renderProjectBoundaryLines();

protected:
	AppController* context;
	RayTracer rayTracer;
};