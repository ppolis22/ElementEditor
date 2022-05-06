#include "MoveableSelectionState.h"

#include "../AppController.h"
#include "../Chunk.h"
#include "../engine/Camera.h"

#include <algorithm>

MoveableSelectionState::MoveableSelectionState(AppController* context)
	: BaseEditorState(context),
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 25.0f),
	moveVector{ 0, 0, 0 },
	isMoving(false)
{}

void MoveableSelectionState::init() {
	BaseEditorState::init();
	handles.setPosition(getHandlePositionForSelection());
}

void MoveableSelectionState::processMouseDown(MouseButtonDownEvent& event) {
	if (event.buttonCode != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	moveDirection = getHandleAtPoint(event.posX, event.posY);
	if (moveDirection != NONE) {
		handles.setSelectedDirection(moveDirection);
		// store closest point from mouse ray to corresponding handle ray
		movementReferencePoint = getClosestPointOnAxisToMouse(event.posX, event.posY);
		handleGrabPointOffset = movementReferencePoint - handles.getPosition();
		isMoving = true;
		event.isHandled = true;
	}
}

void MoveableSelectionState::processMouseUp(MouseButtonUpEvent& event) {
	moveDirection = NONE;
	handles.setSelectedDirection(NONE);
	handles.setPosition(getHandlePositionForSelection());

	if (isMoving) {
		event.isHandled = true;
		isMoving = false;
	}
}

void MoveableSelectionState::processMouseMovement(MouseMoveEvent& event) {
	if (moveDirection != NONE) {
		glm::vec3 pointOnAxis = getClosestPointOnAxisToMouse(event.rawX, event.rawY);
		handles.setPosition(pointOnAxis - handleGrabPointOffset);
		float deltaX = std::floor(pointOnAxis.x + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.x);
		float deltaY = std::floor(pointOnAxis.y + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.y);
		float deltaZ = std::floor(pointOnAxis.z + Chunk::HALF_BLOCK_WIDTH - movementReferencePoint.z);

		if (std::max({ std::abs(deltaX), std::abs(deltaY), std::abs(deltaZ) }) > 0.0f) {
			moveVector += Point3di{ (int)deltaX, (int)deltaY, (int)deltaZ };
			movementReferencePoint += glm::vec3(deltaX, deltaY, deltaZ);
			onMovement();
		}
	}
	//else {
	//	Direction hoveredDirection = getHandleAtPoint(event.rawX, event.rawY);
	//	// highlight axis handle for hoveredDirection?
	//}
}

void MoveableSelectionState::render() {
	renderModelChunks();
	renderLightIcons();

	ModelRenderer* modelRenderer = context->getModelRenderer();
	Camera* camera = context->getCamera();
	LightManager* lightManager = context->getLightManager();
	std::vector<Light*> lights = lightManager->getLights();

	// clear depth buffer to ensure handles are rendered on top of the model
	glClear(GL_DEPTH_BUFFER_BIT);
	Shader& handlesShader = handles.getShader();
	std::vector<Renderable*> toRender;
	toRender.push_back(&handles);
	modelRenderer->renderNoLighting(toRender, handlesShader, *camera, 1.0f);
}

Direction MoveableSelectionState::getHandleAtPoint(float x, float y) {
	Camera* camera = context->getCamera();
	glm::vec3 mouseDirVector = rayTracer.getRayFromScreenCoords(camera->getViewMatrix(), x, y);

	float distToXHandle = rayTracer.getDistanceToTarget(handles.getXBoundingBox(), camera->getPosition(), mouseDirVector);
	float distToYHandle = rayTracer.getDistanceToTarget(handles.getYBoundingBox(), camera->getPosition(), mouseDirVector);
	float distToZHandle = rayTracer.getDistanceToTarget(handles.getZBoundingBox(), camera->getPosition(), mouseDirVector);

	float minDistToHandle = std::min({ distToXHandle , distToYHandle, distToZHandle });
	if (minDistToHandle == RayTracer::MAX_DISTANCE) {
		return NONE;
	} else if (minDistToHandle == distToXHandle) {
		return X;
	} else if (minDistToHandle == distToYHandle) {
		return Y;
	} else {
		return Z;
	}
}

static float calculateTerm(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC, const glm::vec3& ptD) {
	return (ptA.x - ptB.x) * (ptC.x - ptD.x) + (ptA.y - ptB.y) * (ptC.y - ptD.y) + (ptA.z - ptB.z) * (ptC.z - ptD.z);
}

// based on http://paulbourke.net/geometry/pointlineplane/, the several computed terms serve as a simple solution to 
// a system of equations to solve for the parametric eq variable t to get the target distance along line 1
static glm::vec3 closestPointOnLineToOtherLine(const glm::vec3& line1Pt1, const glm::vec3& line1Dir,
	const glm::vec3& line2Pt1, const glm::vec3& line2Dir) {
	glm::vec3 line1Pt2 = line1Pt1 + line1Dir;
	glm::vec3 line2Pt2 = line2Pt1 + line2Dir;

	float term1343 = calculateTerm(line1Pt1, line2Pt1, line2Pt2, line2Pt1);
	float term4321 = calculateTerm(line2Pt2, line2Pt1, line1Pt2, line1Pt1);
	float term1321 = calculateTerm(line1Pt1, line2Pt1, line1Pt2, line1Pt1);
	float term4343 = calculateTerm(line2Pt2, line2Pt1, line2Pt2, line2Pt1);
	float term2121 = calculateTerm(line1Pt2, line1Pt1, line1Pt2, line1Pt1);

	float t = (term1343 * term4321 - term1321 * term4343) / (term2121 * term4343 - term4321 * term4321);
	return line1Dir * t + line1Pt1;
}

glm::vec3 MoveableSelectionState::getClosestPointOnAxisToMouse(float mouseX, float mouseY) {
	Camera* camera = context->getCamera();
	glm::vec3 mouseDirVector = rayTracer.getRayFromScreenCoords(camera->getViewMatrix(), mouseX, mouseY);
	return closestPointOnLineToOtherLine(handles.getPosition(), handles.getAxisVector(moveDirection), camera->getPosition(), mouseDirVector);
}