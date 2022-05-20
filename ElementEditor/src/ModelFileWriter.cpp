#include "ModelFileWriter.h"

ModelFileWriter::ModelFileWriter(ChunkManager& chunkManager, LightManager& lightManager, ProjectBounds& projectBounds, Camera& camera)
	: chunkManager(chunkManager), lightManager(lightManager), projectBounds(projectBounds), camera(camera)
{}

void ModelFileWriter::write() {
	std::ofstream outputFile(FILE_PATH);
	writeToFile(outputFile);
}

void ModelFileWriter::writeToFile(std::ofstream& outputFile) {
	writeColors(outputFile);
	writeBlocks(outputFile);
}

void ModelFileWriter::writeColors(std::ofstream& outputFile) {
	outputFile << sectionTitles[FileSection::COLORS] << std::endl;
	for (Chunk* chunk : chunkManager.getAllChunks()) {
		for (auto block : *chunk) {
			if (!block.isEmpty() && seenColors.count(block) == 0) {
				outputFile << numSeenColors << " " << (int)block.r << " " << (int)block.g << " " << (int)block.b << std::endl;
				seenColors[block] = numSeenColors++;
			}
		}
	}
}

void ModelFileWriter::writeBlocks(std::ofstream& outputFile) {
	outputFile << sectionTitles[FileSection::BLOCKS] << std::endl;
	for (Chunk* chunk : chunkManager.getAllChunks()) {
		for (auto it = chunk->begin(), end = chunk->end(); it != end; ++it) {
			if (!it->isEmpty() && seenColors.count(*it) != 0) {
				outputFile << seenColors[*it] << " " << it.x << " " << it.y << " " << it.z << std::endl;
			}
		}
	}
}