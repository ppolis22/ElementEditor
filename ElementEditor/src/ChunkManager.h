#pragma once

#include <vector>

#include"Chunk.h"

class ChunkManager {
public:
	ChunkManager();
	~ChunkManager();

	void loadChunkData();
	void updateChunkMeshes();
	void renderChunks();

private:
	std::vector<Chunk> allChunks;
	std::vector<Chunk> chunksToUpdate;
};