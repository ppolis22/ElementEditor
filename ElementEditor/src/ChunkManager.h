#pragma once

#include <vector>
#include <unordered_map>

#include "Chunk.h"
#include "engine/Point3d.h"
#include "Block.h"

class ChunkManager {
public:
	ChunkManager();
	~ChunkManager();

	static const int CHUNK_SIZE = 4;

	void setBlock(BlockType type, Point3di location);
	void setSelected(bool isSelected, Point3di location);
	BlockType getBlock(Point3di location);
	std::vector<Chunk> getAllChunks();
	void rebuildChunkMeshes();
	void clear();
	std::unordered_map<Point3di, BlockType, Point3di::HashFunction> getSelected();
	void selectAll();
	void deselectAll();

private:
	std::unordered_map<Point3di, Chunk, Point3di::HashFunction> allChunks;
	std::vector<Chunk*> chunksToRebuild;	// TODO make this a set!

	Point3di getChunkOrigin(Point3di location);
};