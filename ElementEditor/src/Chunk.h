#pragma once

#include "Block.h"

class Chunk {
public:
	static const int CHUNK_SIZE = 16;

	Chunk();
	~Chunk();

	void render(/* Renderer renderer */);
private:
	Block*** data;
};