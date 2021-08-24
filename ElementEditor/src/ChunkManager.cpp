#include "ChunkManager.h"

ChunkManager::ChunkManager() {}

ChunkManager::~ChunkManager() {}

void ChunkManager::setBlock(BlockType type, Point3di location) {
	int chunkX = location.x - ((CHUNK_SIZE + (location.x % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkY = location.y - ((CHUNK_SIZE + (location.y % CHUNK_SIZE)) % CHUNK_SIZE);
	int chunkZ = location.z - ((CHUNK_SIZE + (location.z % CHUNK_SIZE)) % CHUNK_SIZE);

	if (allChunks.find({ chunkX, chunkY, chunkZ }) == allChunks.end()) {
		allChunks.emplace(location, Chunk(chunkX, chunkY, chunkZ));
	}
	Chunk& targetChunk = allChunks.find({ chunkX, chunkY, chunkZ })->second;
	targetChunk.setBlock(type, { location.x - chunkX, location.y - chunkY, location.z - chunkZ });
	chunksToRebuild.push_back(&targetChunk);	// TODO add neighboring chunks too if applicable!
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