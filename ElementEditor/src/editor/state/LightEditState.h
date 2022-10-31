#pragma once

#include "BaseEditorState.h"
#include "MoveableSelectionState.h"
#include "engine/light/Light.h"

class LightEditState : public BaseEditorState {

	// nested class to manage light movement
	class MoveLightState : public MoveableSelectionState {
	public:
		MoveLightState(AppController* context, Light* lightToMove);
		State getType() override;

	private:
		glm::vec3 getHandlePositionForSelection() override;
		void onMovement() override;

		Light* lightToMove;
		Point3di initialLightPosition;
	};

public:
	LightEditState(AppController* context);

	State getType() override;
	void onSelectedLightChanged();

private:
	void processMouseDown(MouseButtonDownEvent& event) override;
	void processMouseUp(MouseButtonUpEvent& event) override;
	void processMouseMovement(MouseMoveEvent& event) override;
	void cleanUp() override;
	void render() override;

	void searchBlocksOnRay(const std::vector<Point3di>& intersectedBlocks);
	void setNewMoveLightState(Light* lightToMove);

	MoveLightState* moveLightState;
};