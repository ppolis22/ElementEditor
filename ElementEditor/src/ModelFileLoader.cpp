#include "ModelFileLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

ModelFileLoader::ModelFileLoader(ChunkManager& chunkManager, LightManager& lightManager, ProjectBounds& projectBounds, Camera& camera)
	: chunkManager(chunkManager), lightManager(lightManager), projectBounds(projectBounds), camera(camera)
{}

static std::vector<std::string> parseLine(std::string& inputString) {
	std::vector<std::string> tokens;
	std::stringstream strStream(inputString);
	std::string token;

	while (getline(strStream, token, ' ')) {
		tokens.push_back(token);
	}
	
	return tokens;
}

void ModelFileLoader::load() {
	std::ifstream inputFile(FILE_PATH);

	if (inputFile.is_open()) { 
		processFile(inputFile);
	}
	else {
		std::ofstream(FILE_PATH);
	}
}

void ModelFileLoader::processFile(std::ifstream& inputFile) {
	std::string line;
	currentSection = FileSection::NONE;

	while (inputFile) {
		std::getline(inputFile, line);
		std::vector<std::string> contents = parseLine(line);

		if (contents.empty())
			continue;

		FileSection newSection = tryParseSection(contents[0]);
		if (newSection != FileSection::NONE) {
			currentSection = newSection;
			continue;
		}
		
		switch (currentSection) {
			case FileSection::COLORS:
				parseColor(contents);
				break;
			case FileSection::BLOCKS:
				parseBlock(contents);
				break;
			case FileSection::LIGHTS:
				parseLight(contents);
				break;
			case FileSection::BOUNDS:
				parseBounds(contents);
				break;
			case FileSection::CAMERA:
				parseCameraData(contents);
				break;
		}
	}
}

ModelFileHandler::FileSection ModelFileLoader::tryParseSection(std::string& str) {
	for (auto& section : sectionTitles) {
		if (str == section.second) {
			return section.first;
		}
	}
	return FileSection::NONE;
}

void ModelFileLoader::parseColor(std::vector<std::string>& contents) {
	if (contents.size() != 4)
		return;

	int index = std::stoi(contents[0]);
	colors[index] = BlockColor{ (unsigned char)stoi(contents[1]), (unsigned char)stoi(contents[2]), (unsigned char)stoi(contents[3]) };
}

void ModelFileLoader::parseBlock(std::vector<std::string>& contents) {
	if (contents.size() != 4)
		return;

	int colorIndex = std::stoi(contents[0]);
	BlockColor color = colors.count(colorIndex) ? colors[colorIndex] : BlockColor::EMPTY();
	chunkManager.setBlockColor(color, Point3di{ std::stoi(contents[1]), std::stoi(contents[2]), std::stoi(contents[3]) }
	);
}

void ModelFileLoader::parseLight(std::vector<std::string>& contents) {
	if (contents.size() == 4) {
		// read in ambient/directional light data
		if (contents[0] == subSectionTitles[FileSubSection::LIGHT_AMBIENT_COLOR]) {
			lightManager.setAmbientLightColor(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
		} else if (contents[0] == subSectionTitles[FileSubSection::LIGHT_DIRECTIONAL_COLOR]) {
			lightManager.setDirectionalLightColor(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
		} else if (contents[0] == subSectionTitles[FileSubSection::LIGHT_DIRECTIONAL_POSITION]) {
			lightManager.setDirectionalLightPosition(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
		}
	} else if (contents.size() == 7) {
		// read in point light data
		glm::vec3 color(std::stof(contents[0]), std::stof(contents[1]), std::stof(contents[2]));
		Point3di position{ std::stoi(contents[3]), std::stoi(contents[4]), std::stoi(contents[5]) };
		lightManager.addLight(color, position, std::stof(contents[6]));
	}
}

void ModelFileLoader::parseBounds(std::vector<std::string>& contents) {
	if (contents.size() != 3)
		return;

	projectBounds.setProjectBounds(std::stoi(contents[0]), std::stoi(contents[1]), std::stoi(contents[2]));
}

void ModelFileLoader::parseCameraData(std::vector<std::string>& contents) {
	if (contents.size() != 4)
		return;

	if (contents[0] == subSectionTitles[FileSubSection::CAMERA_POSITION]) {
		camera.setPosition(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
	} else if (contents[0] == subSectionTitles[FileSubSection::CAMERA_TARGET]) {
		camera.setTarget(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
	}
}