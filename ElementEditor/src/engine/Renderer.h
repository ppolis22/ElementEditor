#pragma once

#include "Renderable.h"
#include "Camera.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void render(Renderable& renderable, Camera& camera);
};