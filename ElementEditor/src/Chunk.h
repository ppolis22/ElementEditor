#pragma once

#include "Block.h"
#include "engine/Point3d.h"
#include "engine/MeshBuilder.h"
#include "engine/Mesh.h"
#include "engine/Shader.h"

class Chunk {
public:
	static const int CHUNK_SIZE = 4;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_BLOCK_WIDTH;

	Chunk() = delete;
	Chunk(int xPos, int yPos, int zPos);
	~Chunk();

	void rebuildMesh();
	void setBlock(BlockType type, int x, int y, int z);
	Shader& getShader();
	Mesh& getMesh();

private:
	BlockType*** data;
	MeshBuilder meshBuilder;
	Mesh mesh;
	Shader chunkShader;
	int xPosition, yPosition, zPosition;

	void buildBlockMesh(int x, int y, int z);
};