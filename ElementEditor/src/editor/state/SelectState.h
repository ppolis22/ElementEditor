#pragma once

#include "BaseEditorState.h"
#include "engine/geom/Point3d.h"

class SelectState : public BaseEditorState {
public:
	SelectState(AppController* context);

	State getType() override;

private:
	void processMouseUp(MouseButtonUpEvent& event) override;
	void searchBlocksOnRay(const std::vector<Point3di>& intersectedBlocks);
	void addBlockToSelection(Point3di& block);
	void removeBlockFromSelection(Point3di& block);
};