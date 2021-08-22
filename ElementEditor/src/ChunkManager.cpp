#include "ChunkManager.h"

ChunkManager::ChunkManager() {}

ChunkManager::~ChunkManager() {}

void ChunkManager::setBlock(BlockType type, Point3di location) {
	int chunkX = location.x - (location.x % CHUNK_SIZE);
	int chunkY = location.y - (location.y % CHUNK_SIZE);
	int chunkZ = location.z - (location.z % CHUNK_SIZE);

	if (allChunks.find({ chunkX, chunkY, chunkZ }) == allChunks.end()) {
		allChunks.emplace(location, Chunk(chunkX, chunkY, chunkZ));
	}
	Chunk& targetChunk = allChunks.find({ chunkX, chunkY, chunkZ })->second;	// should always succeed
	targetChunk.setBlock(type, { location.x - chunkX, location.y - chunkY, location.z - chunkZ });
	targetChunk.rebuildMesh();		// should also have neighboring chunks rebuild meshes?
}

std::vector<Chunk> ChunkManager::getAllChunks() {
	std::vector<Chunk> chunkList;
	for (auto it = allChunks.begin(); it != allChunks.end(); ++it) {
		chunkList.push_back(it->second);
	}
	return chunkList;
}

void ChunkManager::rebuildChunkMeshes() {
	//for ()
}