#pragma once

#include "UIElement.h"
#include "../Mesh.h"
#include "../MeshBuilder.h"

class UIRenderer {
public:
	UIRenderer();
	~UIRenderer();

	void render(UIElement& element);

private:
	Mesh quad;
};