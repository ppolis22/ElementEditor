#pragma once

#include "Block.h"
#include "engine/Point3d.h"
#include "engine/Renderer.h"
#include "engine/MeshBuilder.h"
#include "engine/Mesh.h"

class Chunk {
public:
	static const int CHUNK_SIZE = 16;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_BLOCK_WIDTH;

	Chunk() = delete;
	Chunk(int xPos, int yPos, int zPos);
	~Chunk();

	void render(Renderer& renderer);
	void setBlock(BlockType type, int x, int y, int z);

private:
	BlockType*** data;
	Mesh mesh;
	int xPosition, yPosition, zPosition;

	void rebuildMesh(MeshBuilder& meshBuilder);
	void buildBlockMesh(int x, int y, int z, MeshBuilder& meshBuilder);
};