#pragma once

#include "editor/model/Chunk.h"
#include "editor/model/Block.h"

#include "engine/geom/Point3d.h"
#include "engine/render/Shader.h"

#include <vector>
#include <set>
#include <unordered_map>

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
	Point3di getChunkLocationFromBlockLocation(Point3di blockLocation);
	void flagNeighborChunksToRebuild(Point3di globalLocation, Point3di localLocation);
	void flagChunkToRebuild(Point3di globalLocation);
};