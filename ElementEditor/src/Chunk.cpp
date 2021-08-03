#include "Chunk.h"

const float Chunk::BLOCK_RENDER_SIZE = 1.0f;
const float Chunk::HALF_BLOCK_WIDTH = Chunk::BLOCK_RENDER_SIZE / 2.0f;

Chunk::Chunk(int xPos, int yPos, int zPos) :
	xPosition(xPos),
	yPosition(yPos),
	zPosition(zPos),
	mesh({0, 0, 0}),
	chunkShader("shaders/chunkVertex.shader", "shaders/chunkFragment.shader") {
	data = new BlockType** [CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		data[i] = new BlockType* [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			data[i][j] = new BlockType[CHUNK_SIZE];
			for (int k = 0; k < CHUNK_SIZE; k++) {
				data[i][j][k] = Empty;
			}
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

	meshBuilder.deleteMesh(mesh);
}

void Chunk::setBlock(BlockType type, int x, int y, int z) {
	data[x][y][z] = type;		// should we check if indices are in bounds?
}

Shader& Chunk::getShader() {
	return chunkShader;
}

Mesh& Chunk::getMesh() {
	return mesh;
}

glm::mat4 Chunk::getTransformation() {
	return glm::rotate(glm::mat4(1.0f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));		// arbitrarily rotated for now
}

void Chunk::rebuildMesh() {
	meshBuilder.deleteMesh(mesh);
	meshBuilder.createNewMesh();

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (data[x][y][z] != Empty) {
					buildBlockMesh(x, y, z);
				}
			}
		}
	}

	mesh = meshBuilder.commitMesh();
}

void Chunk::buildBlockMesh(int x, int y, int z) {
	BlockType type = data[x][y][z];

	float xCoord = x * BLOCK_RENDER_SIZE;	// TODO factor in chunk position (i.e. xPosition...)
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

	if ((x == 0 && !checkNeighborChunk(x - 1, y, z)) || (x != 0 && data[x - 1][y][z] == Empty)) {
		Point3df leftNormal{ -1.0f, 0.0f, 0.0f };
		meshBuilder.addFace(leftBottomFar, leftBottomNear, leftTopNear, leftTopFar, leftNormal, type);
	}

	if ((x == CHUNK_SIZE - 1 && !checkNeighborChunk(x + 1, y, z)) || (x != CHUNK_SIZE - 1 && data[x + 1][y][z] == Empty)) {
		Point3df rightNormal{ 1.0f, 0.0f, 0.0f };
		meshBuilder.addFace(rightBottomNear, rightBottomFar, rightTopFar, rightTopNear, rightNormal, type);
	}

	if ((z == 0 && !checkNeighborChunk(x, y, z - 1)) || (z != 0 && data[x][y][z - 1] == Empty)) {
		Point3df backNormal{ 0.0f, 0.0f, -1.0f };
		meshBuilder.addFace(rightBottomFar, leftBottomFar, leftTopFar, rightTopFar, backNormal, type);
	}

	if ((z == CHUNK_SIZE - 1 && !checkNeighborChunk(x, y, z + 1)) || (z != CHUNK_SIZE - 1 && data[x][y][z + 1] == Empty)) {
		Point3df frontNormal{ 0.0f, 0.0f, 1.0f };
		meshBuilder.addFace(leftBottomNear, rightBottomNear, rightTopNear, leftTopNear, frontNormal, type);
	}

	if ((y == 0 && !checkNeighborChunk(x, y - 1, z)) || (y != 0 && data[x][y - 1][z] == Empty)) {
		Point3df bottomNormal{ 0.0f, -1.0f, 0.0f };
		meshBuilder.addFace(leftBottomFar, rightBottomFar, rightBottomNear, leftBottomNear, bottomNormal, type);
	}

	if ((y == CHUNK_SIZE - 1 && !checkNeighborChunk(x, y + 1, z)) || (y != CHUNK_SIZE - 1 && data[x][y + 1][z] == Empty)) {
		Point3df topNormal{ 0.0f, 1.0f, 0.0f };
		meshBuilder.addFace(leftTopNear, rightTopNear, rightTopFar, leftTopFar, topNormal, type);
	}
}

bool Chunk::checkNeighborChunk(int x, int y, int z) {
	return false;
}