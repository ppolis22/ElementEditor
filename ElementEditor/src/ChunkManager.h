#pragma once

#include <vector>
#include <set>
#include <unordered_map>

#include "Chunk.h"
#include "engine/Point3d.h"
#include "Block.h"
#include "engine/Shader.h"

class ChunkManager {
public:
	ChunkManager();
	~ChunkManager();

	static const int CHUNK_SIZE = 4;

	void setBlockColor(BlockColor color, Point3di location);
	void setSelected(bool isSelected, Point3di location);
	BlockColor getBlockColor(Point3di location);
	Shader& getChunkShader();
	std::vector<Chunk*> getAllChunks();
	void rebuildChunkMeshes();
	void clear();
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> getSelected();
	void selectAll();
	void deselectAll();

private:
	std::unordered_map<Point3di, Chunk*, Point3di::HashFunction> allChunks;
	std::set<Chunk*> chunksToRebuild;
	Shader chunkShader;

	Point3di getChunkOrigin(Point3di location);
};