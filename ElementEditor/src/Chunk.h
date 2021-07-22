#pragma once

#include "Block.h"
#include "engine/Point3d.h"
#include "engine/Renderer.h"
#include "engine/MeshBuilder.h"
#include "engine/Mesh.h"

class Chunk {
public:
	static const int CHUNK_SIZE = 4;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_BLOCK_WIDTH;

	Chunk() = delete;
	Chunk(int xPos, int yPos, int zPos);
	~Chunk();

	void render(Renderer& renderer);
	void rebuildMesh();
	void setBlock(BlockType type, int x, int y, int z);

private:
	BlockType*** data;
	MeshBuilder meshBuilder;
	Mesh mesh;
	int xPosition, yPosition, zPosition;

	void buildBlockMesh(int x, int y, int z);
};