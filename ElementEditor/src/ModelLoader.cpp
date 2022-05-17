#include "ModelLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

ModelLoader::ModelLoader(ChunkManager& chunkManager, LightManager& lightManager, ProjectBounds& projectBounds, Camera& camera)
	: chunkManager(chunkManager), lightManager(lightManager), projectBounds(projectBounds), camera(camera)
{
	sections[Section::COLORS] = ("COLORS");
	sections[Section::BLOCKS] = ("BLOCKS");
	sections[Section::LIGHTS] = ("LIGHTS");
	sections[Section::BOUNDS] = ("BOUNDS");
	sections[Section::CAMERA] = ("CAMERA");

	subSections[SubSection::LIGHT_AMBIENT_COLOR] = ("LIGHT_AMBIENT_COLOR");
	subSections[SubSection::LIGHT_DIRECTIONAL_COLOR] = ("LIGHT_DIRECTIONAL_COLOR");
	subSections[SubSection::LIGHT_DIRECTIONAL_POSITION] = ("LIGHT_DIRECTIONAL_POSITION");
	subSections[SubSection::CAMERA_POSITION] = ("CAMERA_POSITION");
	subSections[SubSection::CAMERA_TARGET] = ("CAMERA_TARGET");
}

static std::vector<std::string> parseLine(std::string& inputString) {
	std::vector<std::string> tokens;
	std::stringstream strStream(inputString);
	std::string token;

	while (getline(strStream, token, ' ')) {
		tokens.push_back(token);
	}
	
	return tokens;
}

void ModelLoader::load() {
	std::ifstream inputFile("projects/sample-project.elem");

	if (inputFile.is_open()) { 
		processFile(inputFile);
	}
	else {
		std::ofstream("projects/sample-project.elem");
	}
}

ModelLoader::Section ModelLoader::tryParseSection(std::string& str) {
	for (auto& section : sections) {
		if (str == section.second) {
			return section.first;
		}
	}
	return Section::NONE;
}

void ModelLoader::processFile(std::ifstream& inputFile) {
	std::string line;
	currentSection = Section::NONE;

	while (inputFile) {
		std::getline(inputFile, line);
		std::vector<std::string> contents = parseLine(line);

		if (contents.empty())
			continue;

		Section newSection = tryParseSection(contents[0]);
		if (newSection != Section::NONE) {
			currentSection = newSection;
			continue;
		}
		
		switch (currentSection) {
			case Section::COLORS:
				parseColor(contents);
				break;
			case Section::BLOCKS:
				parseBlock(contents);
				break;
			case Section::LIGHTS:
				parseLight(contents);
				break;
			case Section::BOUNDS:
				parseBounds(contents);
				break;
			case Section::CAMERA:
				parseCameraData(contents);
				break;
		}
	}
}

void ModelLoader::parseColor(std::vector<std::string>& contents) {
	if (contents.size() != 4)
		return;

	int index = std::stoi(contents[0]);
	colors[index] = BlockColor{ (unsigned char)stoi(contents[1]), (unsigned char)stoi(contents[2]), (unsigned char)stoi(contents[3]) };
}

void ModelLoader::parseBlock(std::vector<std::string>& contents) {
	if (contents.size() != 4)
		return;

	int colorIndex = std::stoi(contents[0]);
	BlockColor color = colors.count(colorIndex) ? colors[colorIndex] : BlockColor();
	chunkManager.setBlockColor(color, Point3di{ std::stoi(contents[1]), std::stoi(contents[2]), std::stoi(contents[3]) }
	);
}

void ModelLoader::parseLight(std::vector<std::string>& contents) {
	if (contents.size() == 4) {
		// read in ambient/directional light data
		if (contents[0] == subSections[SubSection::LIGHT_AMBIENT_COLOR]) {
			lightManager.setAmbientLightColor(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
		} else if (contents[0] == subSections[SubSection::LIGHT_DIRECTIONAL_COLOR]) {
			lightManager.setDirectionalLightColor(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
		} else if (contents[0] == subSections[SubSection::LIGHT_DIRECTIONAL_POSITION]) {
			lightManager.setDirectionalLightPosition(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
		}
	} else if (contents.size() == 5) {
		// read in point light data
		int colorIndex = std::stoi(contents[0]);
		BlockColor color = colors.count(colorIndex) ? colors[colorIndex] : BlockColor();
		Point3di position{ std::stoi(contents[1]), std::stoi(contents[2]), std::stoi(contents[3]) };
		lightManager.addLight(
			glm::vec3(color.getNormalizedR(), color.getNormalizedG(), color.getNormalizedB()),
			position,
			std::stof(contents[4]));
	}
}

void ModelLoader::parseBounds(std::vector<std::string>& contents) {
	if (contents.size() != 3)
		return;

	projectBounds.setProjectBounds(std::stoi(contents[0]), std::stoi(contents[1]), std::stoi(contents[2]));
}

void ModelLoader::parseCameraData(std::vector<std::string>& contents) {
	if (contents.size() != 4)
		return;

	if (contents[0] == subSections[SubSection::CAMERA_POSITION]) {
		camera.setPosition(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
	} else if (contents[0] == subSections[SubSection::CAMERA_TARGET]) {
		camera.setTarget(glm::vec3(std::stof(contents[1]), std::stof(contents[2]), std::stof(contents[3])));
	}
}