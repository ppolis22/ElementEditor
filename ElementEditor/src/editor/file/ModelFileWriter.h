#pragma once

#include "ModelFileHandler.h"
#include "editor/controller/ChunkManager.h"
#include "editor/controller/LightManager.h"
#include "editor/model/ProjectBounds.h"

#include "engine/render/Camera.h"

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
	void writeLights(std::ofstream& outputFile);
	void writeBounds(std::ofstream& outputFile);
	void writeCameraData(std::ofstream& outputFile);

	ChunkManager& chunkManager;
	LightManager& lightManager;
	ProjectBounds& projectBounds;
	Camera& camera;

	int numSeenColors = 0;
	std::map<BlockColor, int> seenColors;
};