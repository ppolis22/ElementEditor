#include "ChunkManager.h"

ChunkManager::ChunkManager() {}

ChunkManager::~ChunkManager() {}

void ChunkManager::setBlock(BlockType type, Point3di location) {
	int chunkX = location.x - ((CHUNK_SIZE + (location.x % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkY = location.y - ((CHUNK_SIZE + (location.y % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkZ = location.z - ((CHUNK_SIZE + (location.z % CHUNK_SIZE)) % CHUNK_SIZE);
	Point3di targetChunkOrigin = { chunkX, chunkY, chunkZ };

	if (allChunks.find(targetChunkOrigin) == allChunks.end()) {
		allChunks.emplace(targetChunkOrigin, Chunk(chunkX, chunkY, chunkZ));
	}
	Chunk& targetChunk = allChunks.find(targetChunkOrigin)->second;
	targetChunk.setBlock(type, { location.x - chunkX, location.y - chunkY, location.z - chunkZ });
	chunksToRebuild.push_back(&targetChunk);	// TODO add neighboring chunks too if applicable!
}

void ChunkManager::setSelected(bool selected, Point3di location) {
	Point3di targetChunkOrigin = getChunkOrigin(location);
	if (allChunks.find(targetChunkOrigin) == allChunks.end()) {
		return;
	}
	Chunk& targetChunk = allChunks.find(targetChunkOrigin)->second;
	if (selected) {
		targetChunk.addSelection({ location.x - targetChunkOrigin.x, location.y - targetChunkOrigin.y, location.z - targetChunkOrigin.z });
	} else {
		targetChunk.removeSelection({ location.x - targetChunkOrigin.x, location.y - targetChunkOrigin.y, location.z - targetChunkOrigin.z });
	}
	chunksToRebuild.push_back(&targetChunk);
}

std::vector<Point3di> ChunkManager::getSelected() {
	std::vector<Point3di> totalSelection;
	for (const auto& entry : allChunks) {
		Chunk chunk = entry.second;
		std::vector<Point3di> selectionInChunk = chunk.getSelection();
		if (selectionInChunk.size() > 0) {
			totalSelection.insert(totalSelection.end(), selectionInChunk.begin(), selectionInChunk.end());
		}
	}
	return totalSelection;
}

BlockType ChunkManager::getBlock(Point3di location) {
	int chunkX = location.x - ((CHUNK_SIZE + (location.x % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkY = location.y - ((CHUNK_SIZE + (location.y % CHUNK_SIZE)) % CHUNK_SIZE);	
	int chunkZ = location.z - ((CHUNK_SIZE + (location.z % CHUNK_SIZE)) % CHUNK_SIZE);

	auto it = allChunks.find({ chunkX, chunkY, chunkZ });
	if (it == allChunks.end()) {
		return Empty;
	}
	return it->second.getBlock({ location.x - chunkX, location.y - chunkY, location.z - chunkZ });
}

std::vector<Chunk> ChunkManager::getAllChunks() {
	std::vector<Chunk> chunkList;
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
		it->second.unloadMesh();
	}
	allChunks.clear();
}

Point3di ChunkManager::getChunkOrigin(Point3di location) {
	int chunkX = location.x - ((CHUNK_SIZE + (location.x % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkY = location.y - ((CHUNK_SIZE + (location.y % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkZ = location.z - ((CHUNK_SIZE + (location.z % CHUNK_SIZE)) % CHUNK_SIZE);
	return { chunkX, chunkY, chunkZ };
}