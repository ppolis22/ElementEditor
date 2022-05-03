#pragma once

#include "SelectableState.h"
#include "../engine/RayTracer.h"

class SelectState : public SelectableState {
public:
	SelectState(AppController* context);

	void processMouseUp(MouseButtonUpEvent& event) override;
	State getType() override;

private:
	void searchBlocksOnRay(const std::vector<Point3di>& intersectedBlocks);
	void addBlockToSelection(Point3di& block);
	void removeBlockFromSelection(Point3di& block);
};