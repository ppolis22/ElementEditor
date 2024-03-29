#pragma once

#include "ModelFileHandler.h"
#include "editor/controller/ChunkManager.h"
#include "editor/controller/LightManager.h"
#include "editor/model/ProjectBounds.h"

#include "engine/render/Camera.h"

#include <string>
#include <vector>
#include <map>

class ModelFileLoader : protected ModelFileHandler {
public:
	ModelFileLoader(ChunkManager& chunkManager, LightManager& lightManager, ProjectBounds& projectBounds, Camera& camera);

	void load();

private:
	void processFile(std::ifstream& inputFile);
	FileSection tryParseSection(std::string& str);
	void parseColor(std::vector<std::string>& contents);
	void parseBlock(std::vector<std::string>& contents);
	void parseLight(std::vector<std::string>& contents);
	void parseBounds(std::vector<std::string>& contents);
	void parseCameraData(std::vector<std::string>& contents);

	FileSection currentSection;
	std::map<int, BlockColor> colors;

	ChunkManager& chunkManager;
	LightManager& lightManager;
	ProjectBounds& projectBounds;
	Camera& camera;
};