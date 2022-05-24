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
	writeLights(outputFile);
	writeBounds(outputFile);
	writeCameraData(outputFile);
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
		Point3di chunkPos = chunk->getPosition();
		for (auto it = chunk->begin(), end = chunk->end(); it != end; ++it) {
			if (!it->isEmpty() && seenColors.count(*it) != 0) {
				outputFile << seenColors[*it] << " " << it.x + chunkPos.x << " " << it.y + chunkPos .y << " " << it.z + chunkPos.z << std::endl;
			}
		}
	}
}

void ModelFileWriter::writeLights(std::ofstream& outputFile) {
	outputFile << sectionTitles[FileSection::LIGHTS] << std::endl;

	glm::vec3 ambLightColor = lightManager.getAmbientLightColor();
	outputFile << subSectionTitles[FileSubSection::LIGHT_AMBIENT_COLOR] << " " << 
		ambLightColor.r << " " << ambLightColor.g << " " << ambLightColor.b << std::endl;

	DirectionalLight* dirLight = lightManager.getDirectionalLight();
	glm::vec3 dirLightColor = dirLight->getColor();
	outputFile << subSectionTitles[FileSubSection::LIGHT_DIRECTIONAL] << " " <<
		dirLight->getXBound() << " " << dirLight->getYBound() << " " << dirLight->getZBound() << " " <<
		dirLight->getYAngle() << " " << dirLight->getXAngle() << " " <<
		dirLightColor.r << " " << dirLightColor.g << " " << dirLightColor.b << std::endl;

	for (Light* light : lightManager.getLights()) {
		glm::vec3 lightColor = light->getColor();
		Point3di position = light->getBlockPosition();
		outputFile << lightColor.r << " " << lightColor.g << " " << lightColor.b << " " <<
			position.x << " " << position.y << " " << position.z << " " << light->getStrength() << std::endl;
	}
}

void ModelFileWriter::writeBounds(std::ofstream& outputFile) {
	if (projectBounds.isBounded()) {
		outputFile << sectionTitles[FileSection::BOUNDS] << std::endl;
		outputFile << projectBounds.getXDim() << " " << projectBounds.getYDim() << " " << projectBounds.getZDim() << std::endl;
	}
}

void ModelFileWriter::writeCameraData(std::ofstream& outputFile) {
	outputFile << sectionTitles[FileSection::CAMERA] << std::endl;
	glm::vec3 cameraPosition = camera.getPosition();
	outputFile << subSectionTitles[FileSubSection::CAMERA_POSITION] << " " <<
		cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;

	glm::vec3 cameraTarget = camera.getTarget();
	outputFile << subSectionTitles[FileSubSection::CAMERA_TARGET] << " " <<
		cameraTarget.x << " " << cameraTarget.y << " " << cameraTarget.z << std::endl;
}