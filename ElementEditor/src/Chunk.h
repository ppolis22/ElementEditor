#pragma once

#include "Block.h"
#include "engine/Point3d.h"
#include "engine/MeshBuilder3d.h"
#include "engine/Mesh.h"
#include "engine/Shader.h"
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
	void setBlock(BlockType type, Point3di location);
	BlockType getBlock(Point3di location);
	Shader& getShader() override;
	Mesh& getMesh() override;
	glm::mat4 getTransformation() override;

private:
	std::vector<std::vector<std::vector<BlockType>>> data;
	MeshBuilder3d meshBuilder;
	Mesh mesh;
	Shader chunkShader;
	int xPosition, yPosition, zPosition;

	void buildBlockMesh(int x, int y, int z);
	bool checkNeighborChunk(int x, int y, int z);	// placeholder for method, likely in ChunkManager
};