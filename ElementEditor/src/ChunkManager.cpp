#include "ChunkManager.h"

ChunkManager::ChunkManager()
	: chunkShader("shaders/chunkVertex.shader", "shaders/chunkFragment.shader")
{}

ChunkManager::~ChunkManager() {
	for (Chunk* chunk : this->getAllChunks()) {
		delete chunk;		// should Chunks be allowed to live without a ChunkManager?
	}
}

Shader& ChunkManager::getChunkShader() {
	return chunkShader;
}

Point3di ChunkManager::getChunkLocationFromBlockLocation(Point3di blockLocation) {
	int chunkX = blockLocation.x - ((CHUNK_SIZE + (blockLocation.x % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkY = blockLocation.y - ((CHUNK_SIZE + (blockLocation.y % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkZ = blockLocation.z - ((CHUNK_SIZE + (blockLocation.z % CHUNK_SIZE)) % CHUNK_SIZE);

	return { chunkX, chunkY, chunkZ };
}

void ChunkManager::setBlockColor(BlockColor color, Point3di globalLocation) {
	Point3di chunkOrigin = getChunkLocationFromBlockLocation(globalLocation);

	if (allChunks.find(chunkOrigin) == allChunks.end()) {
		Chunk* chunkToBuild = new Chunk(chunkOrigin.x, chunkOrigin.y, chunkOrigin.z, this);
		allChunks.emplace(chunkOrigin, chunkToBuild);
	}
	Chunk* targetChunk = allChunks.find(chunkOrigin)->second;
	Point3di localLocation = globalLocation - chunkOrigin;
	targetChunk->setBlockColor(color, localLocation);
	chunksToRebuild.insert(targetChunk);

	flagNeighborChunksToRebuild(globalLocation, localLocation);
}

// Here neighbor chunks are flagged for rebuild if the added block is within 2 units of the edge.
// This number comes from the current maximum AO effect distance, thus this method should change if
// that algorithm changes.
void ChunkManager::flagNeighborChunksToRebuild(Point3di globalLocation, Point3di localLocation) {
	if (localLocation.x <= 1)
		flagChunkToRebuild({ globalLocation.x - CHUNK_SIZE, globalLocation.y, globalLocation.z });

	if (localLocation.x >= CHUNK_SIZE - 2)
		flagChunkToRebuild({ globalLocation.x + CHUNK_SIZE, globalLocation.y, globalLocation.z });

	if (localLocation.y <= 1)
		flagChunkToRebuild({ globalLocation.x, globalLocation.y - CHUNK_SIZE, globalLocation.z });

	if (localLocation.y >= CHUNK_SIZE - 2)
		flagChunkToRebuild({ globalLocation.x, globalLocation.y + CHUNK_SIZE, globalLocation.z });

	if (localLocation.z <= 1)
		flagChunkToRebuild({ globalLocation.x, globalLocation.y, globalLocation.z - CHUNK_SIZE });

	if (localLocation.z >= CHUNK_SIZE - 2)
		flagChunkToRebuild({ globalLocation.x, globalLocation.y, globalLocation.z + CHUNK_SIZE });
}

void ChunkManager::flagChunkToRebuild(Point3di globalLocation) {
	Point3di chunkOrigin = getChunkLocationFromBlockLocation(globalLocation);
	auto targetChunkMapping = allChunks.find(chunkOrigin);
	if (targetChunkMapping != allChunks.end()) {
		chunksToRebuild.insert(targetChunkMapping->second);
	}
}

void ChunkManager::setSelected(bool selected, Point3di location) {
	Point3di targetChunkOrigin = getChunkOrigin(location);
	if (allChunks.find(targetChunkOrigin) == allChunks.end()) {
		return;
	}
	Chunk* targetChunk = allChunks.find(targetChunkOrigin)->second;
	if (selected) {
		targetChunk->addSelection({ location.x - targetChunkOrigin.x, location.y - targetChunkOrigin.y, location.z - targetChunkOrigin.z });
	} else {
		targetChunk->removeSelection({ location.x - targetChunkOrigin.x, location.y - targetChunkOrigin.y, location.z - targetChunkOrigin.z });
	}
	chunksToRebuild.insert(targetChunk);
}

std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> ChunkManager::getSelected() {
	std::unordered_map<Point3di, BlockColor, Point3di::HashFunction> totalSelection;
	for (const auto& entry : allChunks) {
		Chunk* chunk = entry.second;
		std::vector<Point3di> selectionInChunk = chunk->getSelection();
		for (Point3di& selectedPoint : selectionInChunk) {
			totalSelection.emplace(selectedPoint, this->getBlockColor(selectedPoint));
		}
	}
	return totalSelection;
}

void ChunkManager::selectAll() {
	for (auto& entry : allChunks) {
		entry.second->selectAll();
		chunksToRebuild.insert(entry.second);
	}
	this->rebuildChunkMeshes();
}

void ChunkManager::deselectAll() {
	for (auto& entry : allChunks) {
		entry.second->deselectAll();
		chunksToRebuild.insert(entry.second);
	}
	this->rebuildChunkMeshes();
}

BlockColor ChunkManager::getBlockColor(Point3di location) {
	Point3di chunkOrigin = getChunkLocationFromBlockLocation(location);

	auto it = allChunks.find(chunkOrigin);
	if (it == allChunks.end()) {
		return BlockColor::EMPTY();
	}
	return it->second->getBlockColor(location - chunkOrigin);
}

std::vector<Chunk*> ChunkManager::getAllChunks() {
	std::vector<Chunk*> chunkList;
	for (auto it = allChunks.begin(); it != allChunks.end(); ++it) {
		chunkList.push_back(it->second);
	}
	return chunkList;
}

void ChunkManager::rebuildChunkMeshes() {
	for (Chunk* chunk : chunksToRebuild) {
		chunk->rebuildMesh();		// TODO remove chunk from map if empty mesh
	}
	chunksToRebuild.clear();
}

void ChunkManager::clear() {
	for (auto it = allChunks.begin(); it != allChunks.end(); ++it) {
		it->second->unloadMesh();
	}
	allChunks.clear();
}

Point3di ChunkManager::getChunkOrigin(Point3di location) {
	int chunkX = location.x - ((CHUNK_SIZE + (location.x % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkY = location.y - ((CHUNK_SIZE + (location.y % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkZ = location.z - ((CHUNK_SIZE + (location.z % CHUNK_SIZE)) % CHUNK_SIZE);
	return { chunkX, chunkY, chunkZ };
}