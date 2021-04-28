#include "Chunk.h"

const float Chunk::BLOCK_RENDER_SIZE = 1.0f;
const float Chunk::HALF_BLOCK_WIDTH = Chunk::BLOCK_RENDER_SIZE / 2.0f;

Chunk::Chunk(int xPos, int yPos, int zPos) :
	xPosition(xPos),
	yPosition(yPos),
	zPosition(zPos),
	meshVertexArrayId(0) {
	data = new BlockType** [CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		data[i] = new BlockType* [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			data[i][j] = new BlockType[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() {
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] data[i][j];
		}
		delete[] data[i];
	}
	delete[] data;
}

void Chunk::render(Renderer& renderer) {
	// need to pass along the x, y, and zPosition to create model transformation matrix
}

void Chunk::buildMesh(MeshBuilder& meshBuilder) {	// TODO optimize!!!
	meshBuilder.deleteMesh(meshVertexArrayId);
	meshVertexArrayId = meshBuilder.createNewMesh();

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (data[x][y][z] != Empty) {
					buildBlockMesh(x, y, z, meshBuilder);
				}
			}
		}
	}
}

void Chunk::buildBlockMesh(int x, int y, int z, MeshBuilder& meshBuilder) {
	BlockType type = data[x][y][z];
	float xCoord = x * BLOCK_RENDER_SIZE;
	float yCoord = y * BLOCK_RENDER_SIZE;
	float zCoord = z * BLOCK_RENDER_SIZE;

	float leftX = xCoord - HALF_BLOCK_WIDTH;
	float rightX = xCoord + HALF_BLOCK_WIDTH;
	float downY = yCoord - HALF_BLOCK_WIDTH;
	float upY = yCoord + HALF_BLOCK_WIDTH;
	float nearZ = zCoord + HALF_BLOCK_WIDTH;	// +z toward camera, traditionally
	float farZ = zCoord - HALF_BLOCK_WIDTH;

	Point3df leftBottomNear{leftX, downY, nearZ};
	Point3df rightBottomNear{ rightX, downY, nearZ };
	Point3df leftTopNear{ leftX, upY, nearZ };
	Point3df rightTopNear{ rightX, upY, nearZ };
	Point3df leftBottomFar{ leftX, downY, farZ };
	Point3df rightBottomFar{ rightX, downY, farZ };
	Point3df leftTopFar{ leftX, upY, farZ };
	Point3df rightTopFar{ rightX, upY, farZ };

	Point3df frontNormal{ 0.0f, 0.0f, 1.0f };
	meshBuilder.addFace(leftBottomNear, rightBottomNear, rightTopNear, leftTopNear, frontNormal, type);

	Point3df leftNormal{ -1.0f, 0.0f, 0.0f };
	meshBuilder.addFace(leftBottomFar, leftBottomNear, leftTopNear, leftTopFar, leftNormal, type);

	Point3df rightNormal{ 1.0f, 0.0f, 0.0f };
	meshBuilder.addFace(rightBottomNear, rightBottomFar, rightTopFar, rightTopNear, rightNormal, type);

	Point3df backNormal{ 0.0f, 0.0f, -1.0f };
	meshBuilder.addFace(rightBottomFar, leftBottomFar, leftTopFar, rightTopFar, backNormal, type);

	Point3df topNormal{ 0.0f, 1.0f, 0.0f };
	meshBuilder.addFace(leftTopNear, rightTopNear, rightTopFar, leftTopFar, topNormal, type);

	Point3df bottomNormal{ 0.0f, -1.0f, 0.0f };
	meshBuilder.addFace(leftBottomFar, rightBottomFar, rightBottomNear, leftBottomNear, bottomNormal, type);
}

void Chunk::setBlock(BlockType type, int x, int y, int z) {
	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
		throw "Attempt to set invalid block location.";
	}
	data[x][y][z] = type;
}