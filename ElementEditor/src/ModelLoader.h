#pragma once

#include "ChunkManager.h"
#include "LightManager.h"
#include "ProjectBounds.h"
#include "engine/Camera.h"

#include <string>
#include <vector>
#include <map>

class ModelLoader {
public:
	ModelLoader(ChunkManager& chunkManager, LightManager& lightManager, ProjectBounds& projectBounds, Camera& camera);

	void load();

private:
	enum class Section {
		COLORS,
		BLOCKS,
		LIGHTS,
		BOUNDS,
		CAMERA,
		NONE
	};

	enum class SubSection {
		LIGHT_AMBIENT_COLOR,
		LIGHT_DIRECTIONAL_COLOR,
		LIGHT_DIRECTIONAL_POSITION,
		CAMERA_POSITION,
		CAMERA_TARGET,
		NONE
	};

	void processFile(std::ifstream& inputFile);
	Section tryParseSection(std::string& str);
	void parseColor(std::vector<std::string>& contents);
	void parseBlock(std::vector<std::string>& contents);
	void parseLight(std::vector<std::string>& contents);
	void parseBounds(std::vector<std::string>& contents);
	void parseCameraData(std::vector<std::string>& contents);

	std::map<Section, std::string> sections;
	std::map<SubSection, std::string> subSections;
	Section currentSection;

	std::map<int, BlockColor> colors;

	ChunkManager& chunkManager;
	LightManager& lightManager;
	ProjectBounds& projectBounds;
	Camera& camera;
};