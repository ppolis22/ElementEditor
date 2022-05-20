#pragma once

#include "ModelFileHandler.h"
#include "ChunkManager.h"
#include "LightManager.h"
#include "ProjectBounds.h"
#include "engine/Camera.h"

#include <fstream>
#include <map>

class ModelFileWriter : protected ModelFileHandler {
public:
	ModelFileWriter(ChunkManager& chunkManager, LightManager& lightManager, ProjectBounds& projectBounds, Camera& camera);

	void write();

private:
	void writeToFile(std::ofstream& outputFile);
	void writeColors(std::ofstream& outputFile);
	void writeBlocks(std::ofstream& outputFile);

	ChunkManager& chunkManager;
	LightManager& lightManager;
	ProjectBounds& projectBounds;
	Camera& camera;

	int numSeenColors = 0;
	std::map<BlockColor, int> seenColors;
};