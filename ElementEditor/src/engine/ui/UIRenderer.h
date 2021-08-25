#pragma once

#include "UIElement.h"
#include "../Mesh.h"
#include "../MeshBuilder2d.h"
#include "../Shader.h"

class UIRenderer {
public:
	UIRenderer() = delete;
	UIRenderer(MeshBuilder2d& meshBuilder);
	~UIRenderer();

	void render(UIElement& element);

private:
	Mesh quad;
	Shader shader;
};