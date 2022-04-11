#include "Chunk.h"

#include <algorithm>

const float Chunk::BLOCK_RENDER_SIZE = 1.0f;
const float Chunk::HALF_BLOCK_WIDTH = Chunk::BLOCK_RENDER_SIZE / 2.0f;

Chunk::Chunk(int xPos, int yPos, int zPos) :
	xPosition(xPos),
	yPosition(yPos),
	zPosition(zPos),
	mesh({0, 0, 0, 0}),
	chunkShader("shaders/chunkVertex.shader", "shaders/chunkFragment.shader"),
	data(CHUNK_SIZE, std::vector<std::vector<BlockColor>>(CHUNK_SIZE, std::vector<BlockColor>(CHUNK_SIZE, BlockColor::EMPTY())))
	{}

Chunk::~Chunk() {}

void Chunk::unloadMesh() {
	meshBuilder.deleteMesh(mesh);
}

void Chunk::setBlockColor(BlockColor color, Point3di location) {
	data[location.x][location.y][location.z] = { color.r, color.b, color.g, color.alpha };		// should we check if indices are in bounds?
}

void Chunk::removeBlock(Point3di location) {
	data[location.x][location.y][location.z] = BlockColor::EMPTY();
}

void Chunk::addSelection(Point3di location) {
	if (std::find(selectedBlocks.begin(), selectedBlocks.end(), location) == selectedBlocks.end()) {
		selectedBlocks.push_back(location);
	}
}

void Chunk::removeSelection(Point3di location) {
	selectedBlocks.erase(std::remove(selectedBlocks.begin(), selectedBlocks.end(), location), selectedBlocks.end());
}

void Chunk::selectAll() {
	selectedBlocks.clear();
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (!data[x][y][z].isEmpty()) {
					selectedBlocks.push_back({x, y, z});
				}
			}
		}
	}
}

void Chunk::deselectAll() {
	selectedBlocks.clear();
}

std::vector<Point3di> Chunk::getSelection() {
	std::vector<Point3di> worldCoordsSelection;
	for (Point3di& selection : selectedBlocks) {
		Point3di adjustedPoint{selection.x + xPosition, selection.y + yPosition, selection.z + zPosition};
		worldCoordsSelection.push_back(adjustedPoint);
	}
	return worldCoordsSelection;
}

BlockColor Chunk::getBlockColor(Point3di location) {
	return data[location.x][location.y][location.z];
}

Shader& Chunk::getShader() {
	return chunkShader;
}

Mesh& Chunk::getMesh() {
	return mesh;
}

glm::mat4 Chunk::getTransformation() {
	return glm::mat4(1.0f);		// chunks will likely not individually translate/rotate/scale
}

void Chunk::rebuildMesh() {
	meshBuilder.deleteMesh(mesh);
	meshBuilder.createNewMesh();

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (!data[x][y][z].isEmpty()) {
					buildBlockMesh(x, y, z, data[x][y][z]);
				}
			}
		}
	}

	mesh = meshBuilder.commitMesh();
}

void Chunk::buildBlockMesh(int x, int y, int z, BlockColor color) {
	Point3di location {x, y, z};
	glm::vec3 renderColor((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f);

	if (std::find(selectedBlocks.begin(), selectedBlocks.end(), location) != selectedBlocks.end()) {	
		renderColor = { 1.0, 0.55, 0.16 };		// TODO refernce constant System colors for "selected" color
	}

	float xCoord = x * BLOCK_RENDER_SIZE + xPosition;
	float yCoord = y * BLOCK_RENDER_SIZE + yPosition;
	float zCoord = z * BLOCK_RENDER_SIZE + zPosition;

	float leftX = xCoord;
	float rightX = xCoord + BLOCK_RENDER_SIZE;
	float downY = yCoord;
	float upY = yCoord + BLOCK_RENDER_SIZE;
	float nearZ = zCoord + BLOCK_RENDER_SIZE;	// +z toward camera, traditionally
	float farZ = zCoord;

	Point3df leftBottomNear{leftX, downY, nearZ};
	Point3df rightBottomNear{ rightX, downY, nearZ };
	Point3df leftTopNear{ leftX, upY, nearZ };
	Point3df rightTopNear{ rightX, upY, nearZ };
	Point3df leftBottomFar{ leftX, downY, farZ };
	Point3df rightBottomFar{ rightX, downY, farZ };
	Point3df leftTopFar{ leftX, upY, farZ };
	Point3df rightTopFar{ rightX, upY, farZ };

	if ((x == 0 && !checkNeighborChunk(x - 1, y, z)) || (x != 0 && data[x - 1][y][z].isEmpty())) {
		Point3df leftNormal{ -1.0f, 0.0f, 0.0f };
		meshBuilder.addFace(leftBottomFar, leftBottomNear, leftTopNear, leftTopFar, leftNormal, renderColor);
	}

	if ((x == CHUNK_SIZE - 1 && !checkNeighborChunk(x + 1, y, z)) || (x != CHUNK_SIZE - 1 && data[x + 1][y][z].isEmpty())) {
		Point3df rightNormal{ 1.0f, 0.0f, 0.0f };
		meshBuilder.addFace(rightBottomNear, rightBottomFar, rightTopFar, rightTopNear, rightNormal, renderColor);
	}

	if ((z == 0 && !checkNeighborChunk(x, y, z - 1)) || (z != 0 && data[x][y][z - 1].isEmpty())) {
		Point3df backNormal{ 0.0f, 0.0f, -1.0f };
		meshBuilder.addFace(rightBottomFar, leftBottomFar, leftTopFar, rightTopFar, backNormal, renderColor);
	}

	if ((z == CHUNK_SIZE - 1 && !checkNeighborChunk(x, y, z + 1)) || (z != CHUNK_SIZE - 1 && data[x][y][z + 1].isEmpty())) {
		Point3df frontNormal{ 0.0f, 0.0f, 1.0f };
		meshBuilder.addFace(leftBottomNear, rightBottomNear, rightTopNear, leftTopNear, frontNormal, renderColor);
	}

	if ((y == 0 && !checkNeighborChunk(x, y - 1, z)) || (y != 0 && data[x][y - 1][z].isEmpty())) {
		Point3df bottomNormal{ 0.0f, -1.0f, 0.0f };
		meshBuilder.addFace(leftBottomFar, rightBottomFar, rightBottomNear, leftBottomNear, bottomNormal, renderColor);
	}

	if ((y == CHUNK_SIZE - 1 && !checkNeighborChunk(x, y + 1, z)) || (y != CHUNK_SIZE - 1 && data[x][y + 1][z].isEmpty())) {
		Point3df topNormal{ 0.0f, 1.0f, 0.0f };
		meshBuilder.addFace(leftTopNear, rightTopNear, rightTopFar, leftTopFar, topNormal, renderColor);
	}
}

bool Chunk::checkNeighborChunk(int x, int y, int z) {
	return false;		// TODO consult owning ChunkManager
}