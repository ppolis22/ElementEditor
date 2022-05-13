#pragma once

#include "Block.h"
#include "engine/Point3d.h"
#include "engine/MeshBuilder3d.h"
#include "engine/Mesh.h"
#include "engine/Renderable.h"

#include <vector>

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

class ChunkManager;

class Chunk : public Renderable {
public:
	static const int CHUNK_SIZE = 4;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_BLOCK_WIDTH;

	Chunk() = delete;
	Chunk(int xPos, int yPos, int zPos, ChunkManager* manager);
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

	// It may make more sense to have a static ChunkManager for the entire session, or a static/singleton
	// session object with a static/singleton ChunkManager. For now, this will do.
	ChunkManager* manager;

	void buildBlockMesh(int x, int y, int z, BlockColor color);
	bool blockIsEmpty(int x, int y, int z);
	bool blockIsEmpty(const Point3di& point);
	float calculateOcclusion(const Point3di& vertex, const Point3di& corner, const Point3df& normal);
};