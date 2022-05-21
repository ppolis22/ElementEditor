#pragma once

#include <string>
#include <map>

class ModelFileHandler {
protected:
	ModelFileHandler();

	const std::string FILE_PATH = "projects/project.elem";

	enum class FileSection {
		COLORS,
		BLOCKS,
		LIGHTS,
		BOUNDS,
		CAMERA,
		NONE
	};

	enum class FileSubSection {
		LIGHT_AMBIENT_COLOR,
		LIGHT_DIRECTIONAL_COLOR,
		LIGHT_DIRECTIONAL_POSITION,
		LIGHT_DIRECTIONAL_TARGET,
		CAMERA_POSITION,
		CAMERA_TARGET,
		NONE
	};

	std::map<FileSection, std::string> sectionTitles;

	std::map<FileSubSection, std::string> subSectionTitles;
};