#include "MoveState.h"

#include "engine/ModelRenderer.h"
#include "AppController.h"
#include "ChunkManager.h"
#include "engine/Camera.h"

#include <vector>

MoveState::MoveState(AppController* context, std::vector<Point3di> selection)
	: BaseEditorState(context),
	selection(selection),
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 10.0f) 
{
	handles.setPosition(averagePoints(selection) + Chunk::HALF_BLOCK_WIDTH);
}

void MoveState::processMouseMovement(MouseMoveEvent& event) {
	if (moveDirection != NONE) {
		// move Selection according to moveDirection
	} 
	//else {
	//	Direction hoveredDirection = getHandleAtPoint(event.rawX, event.rawY);
	//	// highlight axis handle for hoveredDirection?
	//}
}

void MoveState::processMouseDown(MouseButtonDownEvent& event) {
	// determine if mouse intersects with one of the axis handles
	moveDirection = getHandleAtPoint(event.posX, event.posY);
	handles.setSelectedDirection(moveDirection);
}

void MoveState::processMouseUp(MouseButtonUpEvent& event) {
	moveDirection = NONE;
	// commit mesh changes if any were made?
	// unhighlight axis handle?
	handles.setSelectedDirection(NONE);
}

void MoveState::render() {
	// render model mesh
	ModelRenderer* modelRenderer = context->getModelRenderer();
	ChunkManager* modelChunkManager = context->getModelChunkManager();
	Camera* camera = context->getCamera();

	for (Chunk& chunk : modelChunkManager->getAllChunks()) {
		modelRenderer->render(chunk, *camera);
	}
	// clear depth buffer to endure handles are rendered on top of the model
	glClear(GL_DEPTH_BUFFER_BIT);
	modelRenderer->render(handles, *camera);
}

Direction MoveState::getHandleAtPoint(float x, float y) {
	Camera* camera = context->getCamera();
	glm::vec3 mouseDirVector = rayTracer.getRayFromScreenCoords(camera->getViewMatrix(), x, y);
	Direction targetDirection = NONE;
	float minDistToHandle = RayTracer::MAX_DISTANCE;

	AABB xHandleBox = handles.getXBoundingBox();
	float distToXHandle = rayTracer.getDistanceToTarget(xHandleBox, context->getCamera()->getPosition(), mouseDirVector);
	if (distToXHandle < minDistToHandle) {
		targetDirection = X;
		minDistToHandle = distToXHandle;
	}

	// TODO repeat for other handles

	return targetDirection;
}

glm::vec3 MoveState::averagePoints(const std::vector<Point3di>& points) {
	glm::vec3 average(0.0f, 0.0f, 0.0f);
	for (const Point3di& point : points) {
		average += glm::vec3((float)point.x, (float)point.y, (float)point.z);
	}
	return average / (float)points.size();
}
