#pragma once

#include "Block.h"
#include "engine/Point3d.h"
#include "engine/MeshBuilder3d.h"
#include "engine/Mesh.h"
#include "engine/Renderable.h"

#include <vector>

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

class Chunk : public Renderable {
public:
	static const int CHUNK_SIZE = 4;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_BLOCK_WIDTH;

	Chunk() = delete;
	Chunk(int xPos, int yPos, int zPos);
	~Chunk();

	void rebuildMesh();
	void unloadMesh();
	void setBlockColor(BlockColor color, Point3di location);
	void removeBlock(Point3di location);
	void addSelection(Point3di location);
	void removeSelection(Point3di location);
	void selectAll();
	void deselectAll();
	std::vector<Point3di> getSelection();
	BlockColor getBlockColor(Point3di location);
	Mesh& getMesh() override;
	glm::mat4 getTransformation() override;

private:
	std::vector<std::vector<std::vector<BlockColor>>> data;
	std::vector <Point3di> selectedBlocks;
	MeshBuilder3d meshBuilder;
	Mesh mesh;
	int xPosition, yPosition, zPosition;

	void buildBlockMesh(int x, int y, int z, BlockColor color);
	bool neighborChunkHasBlock(int x, int y, int z);	// placeholder for method, likely in ChunkManager
	bool blockIsEmpty(int x, int y, int z);
	bool blockIsEmpty(const Point3di& point);
	float calculateOcclusion(const Point3di& vertex, const Point3di& corner, const Point3df& normal);
};