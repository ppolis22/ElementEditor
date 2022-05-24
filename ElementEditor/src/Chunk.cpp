#include "Chunk.h"

#include "ChunkManager.h"
#include <algorithm>

const float Chunk::BLOCK_RENDER_SIZE = 1.0f;
const float Chunk::HALF_BLOCK_WIDTH = Chunk::BLOCK_RENDER_SIZE / 2.0f;

Chunk::Chunk(int xPos, int yPos, int zPos, ChunkManager* manager) :
	xPosition(xPos),
	yPosition(yPos),
	zPosition(zPos), 
	manager(manager),
	mesh({0, 0, 0, 0}),
	data(CHUNK_SIZE, std::vector<std::vector<BlockColor>>(CHUNK_SIZE, std::vector<BlockColor>(CHUNK_SIZE, BlockColor::EMPTY())))
	{}

Chunk::~Chunk() {}

void Chunk::unloadMesh() {
	mesh.deleteMesh();
}

void Chunk::setBlockColor(BlockColor color, Point3di location) {
	data[location.x][location.y][location.z] = color;		// should we check if indices are in bounds?
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

Point3di Chunk::getPosition() {
	return Point3di{ xPosition, yPosition, zPosition };
}

Mesh& Chunk::getMesh() {
	return mesh;
}

glm::mat4 Chunk::getTransformation() {
	return glm::mat4(1.0f);		// chunks will likely not individually translate/rotate/scale
}

void Chunk::rebuildMesh() {
	mesh.deleteMesh();
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
	glm::vec3 renderColor(color.getNormalizedR(), color.getNormalizedG(), color.getNormalizedB());

	if (std::find(selectedBlocks.begin(), selectedBlocks.end(), location) != selectedBlocks.end()) {	
		renderColor = { 1.0f, 0.55f, 0.16f };		// TODO refernce constant System colors for "selected" color
	}

	float leftX = x * BLOCK_RENDER_SIZE + xPosition;
	float downY = y * BLOCK_RENDER_SIZE + yPosition;
	float farZ = z * BLOCK_RENDER_SIZE + zPosition;

	float rightX = leftX + BLOCK_RENDER_SIZE;
	float upY = downY + BLOCK_RENDER_SIZE;
	float nearZ = farZ + BLOCK_RENDER_SIZE;	// +z toward camera, traditionally

	Point3df leftBottomNear{leftX, downY, nearZ};
	Point3df rightBottomNear{ rightX, downY, nearZ };
	Point3df leftTopNear{ leftX, upY, nearZ };
	Point3df rightTopNear{ rightX, upY, nearZ };
	Point3df leftBottomFar{ leftX, downY, farZ };
	Point3df rightBottomFar{ rightX, downY, farZ };
	Point3df leftTopFar{ leftX, upY, farZ };
	Point3df rightTopFar{ rightX, upY, farZ };

	// only build faces that could be visible, i.e. border empty blocks
	if (blockIsEmpty(x - 1, y, z)) {
		Point3df leftNormal{ -1.0f, 0.0f, 0.0f };
		float lbfOcc = calculateOcclusion(location, location + Point3di{-1, -1, -1}, leftNormal);
		float lbnOcc = calculateOcclusion(location, location + Point3di{ -1, -1, 1 }, leftNormal);
		float ltnOcc = calculateOcclusion(location, location + Point3di{ -1, 1, 1 }, leftNormal);
		float ltfOcc = calculateOcclusion(location, location + Point3di{ -1, 1, -1 }, leftNormal);
		meshBuilder.addFace(leftBottomFar, leftBottomNear, leftTopNear, leftTopFar, 
			lbfOcc, lbnOcc, ltnOcc, ltfOcc,
			leftNormal, renderColor);
	}

	if (blockIsEmpty(x + 1, y, z)) {
		Point3df rightNormal{ 1.0f, 0.0f, 0.0f };
		float rbnOcc = calculateOcclusion(location, location + Point3di{ 1, -1, 1 }, rightNormal);
		float rbfOcc = calculateOcclusion(location, location + Point3di{ 1, -1, -1 }, rightNormal);
		float rtfOcc = calculateOcclusion(location, location + Point3di{ 1, 1, -1 }, rightNormal);
		float rtnOcc = calculateOcclusion(location, location + Point3di{ 1, 1, 1 }, rightNormal);
		meshBuilder.addFace(rightBottomNear, rightBottomFar, rightTopFar, rightTopNear, 
			rbnOcc, rbfOcc, rtfOcc, rtnOcc,
			rightNormal, renderColor);
	}

	if (blockIsEmpty(x, y, z - 1)) {
		Point3df backNormal{ 0.0f, 0.0f, -1.0f };
		float rbfOcc = calculateOcclusion(location, location + Point3di{ 1, -1, -1 }, backNormal);
		float lbfOcc = calculateOcclusion(location, location + Point3di{ -1, -1, -1 }, backNormal);
		float ltfOcc = calculateOcclusion(location, location + Point3di{ -1, 1, -1 }, backNormal);
		float rtfOcc = calculateOcclusion(location, location + Point3di{ 1, 1, -1 }, backNormal);
		meshBuilder.addFace(rightBottomFar, leftBottomFar, leftTopFar, rightTopFar, 
			rbfOcc, lbfOcc, ltfOcc, rtfOcc,
			backNormal, renderColor);
	}

	if (blockIsEmpty(x, y, z + 1)) {
		Point3df frontNormal{ 0.0f, 0.0f, 1.0f };
		float lbnOcc = calculateOcclusion(location, location + Point3di{ -1, -1, 1 }, frontNormal);
		float rbnOcc = calculateOcclusion(location, location + Point3di{ 1, -1, 1 }, frontNormal);
		float rtnOcc = calculateOcclusion(location, location + Point3di{ 1, 1, 1 }, frontNormal);
		float ltnOcc = calculateOcclusion(location, location + Point3di{ -1, 1, 1 }, frontNormal);
		meshBuilder.addFace(leftBottomNear, rightBottomNear, rightTopNear, leftTopNear, 
			lbnOcc, rbnOcc, rtnOcc, ltnOcc,
			frontNormal, renderColor);
	}

	if (blockIsEmpty(x, y - 1, z)) {
		Point3df bottomNormal{ 0.0f, -1.0f, 0.0f };
		float lbfOcc = calculateOcclusion(location, location + Point3di{ -1, -1, -1 }, bottomNormal);
		float rbfOcc = calculateOcclusion(location, location + Point3di{ 1, -1, -1 }, bottomNormal);
		float rbnOcc = calculateOcclusion(location, location + Point3di{ 1, -1, 1 }, bottomNormal);
		float lbnOcc = calculateOcclusion(location, location + Point3di{ -1, -1, 1 }, bottomNormal);
		meshBuilder.addFace(leftBottomFar, rightBottomFar, rightBottomNear, leftBottomNear, 
			lbfOcc, rbfOcc, rbnOcc, lbnOcc,
			bottomNormal, renderColor);
	}

	if (blockIsEmpty(x, y + 1, z)) {
		Point3df topNormal{ 0.0f, 1.0f, 0.0f };
		float ltnOcc = calculateOcclusion(location, location + Point3di{ -1, 1, 1 }, topNormal);
		float rtnOcc = calculateOcclusion(location, location + Point3di{ 1, 1, 1 }, topNormal);
		float rtfOcc = calculateOcclusion(location, location + Point3di{ 1, 1, -1 }, topNormal);
		float ltfOcc = calculateOcclusion(location, location + Point3di{ -1, 1, -1 }, topNormal);
		meshBuilder.addFace(leftTopNear, rightTopNear, rightTopFar, leftTopFar, 
			ltnOcc, rtnOcc, rtfOcc, ltfOcc,
			topNormal, renderColor);
	}
}

bool Chunk::blockIsEmpty(int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
		return manager->getBlockColor({ x + xPosition, y + yPosition, z + zPosition }).isEmpty();
	}

	return data[x][y][z].isEmpty();
}

bool Chunk::blockIsEmpty(const Point3di& point) {
	return blockIsEmpty(point.x, point.y, point.z);
}

// TODO come up with more robust/flexible way to do this
//
// This method serves as an approximation of ambient occlusion that is calculated for each vertex when the
// mesh is rebuilt. Currently only accounts for 3 blocks surrounding vertex, and 4 blocks above it.
float Chunk::calculateOcclusion(const Point3di& vertex, const Point3di& corner, const Point3df& normalFloat) {
	Point3di normal{ static_cast<int>(normalFloat.x), static_cast<int>(normalFloat.y), static_cast<int>(normalFloat.z) };
	Point3di toCheck = (vertex - corner) + normal;
	
	float occlusion = 0.0f;
	Point3di ptXShift{ corner.x + toCheck.x, corner.y, corner.z };
	Point3di ptYShift{ corner.x, corner.y + toCheck.y, corner.z };
	Point3di ptZShift{ corner.x, corner.y, corner.z + toCheck.z };

	// account for the fact that the corner won't contribute to occlusion if the two side blocks are present
	if ((ptXShift == corner && !blockIsEmpty(ptYShift) && !blockIsEmpty(ptZShift)) ||
		(ptYShift == corner && !blockIsEmpty(ptXShift) && !blockIsEmpty(ptZShift)) ||
		(ptZShift == corner && !blockIsEmpty(ptXShift) && !blockIsEmpty(ptYShift))) 
	{
		occlusion += 0.6f;
	} else {
		occlusion += ((!blockIsEmpty(ptXShift) ? 0.2f : 0.0f) + (!blockIsEmpty(ptYShift) ? 0.2f : 0.0f) + (!blockIsEmpty(ptZShift) ? 0.2f : 0.0f));
	}

	occlusion += 
		  (!blockIsEmpty(ptXShift + normal) ? 0.10f : 0.0f)		// account for 4 above blocks
		+ (!blockIsEmpty(ptYShift + normal) ? 0.10f : 0.0f)
		+ (!blockIsEmpty(ptZShift + normal) ? 0.10f : 0.0f)
		+ (!blockIsEmpty(vertex.x + normal.x * 2, vertex.y + normal.y * 2, vertex.z + normal.z * 2) ? 0.10f : 0.0f);

	return occlusion;
}