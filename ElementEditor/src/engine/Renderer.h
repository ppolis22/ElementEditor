#pragma once

#include "..\Chunk.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void renderChunk(Chunk& chunk);

private:
	const float FOV = 45.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;
};