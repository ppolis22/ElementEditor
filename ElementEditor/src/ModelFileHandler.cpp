#include "ModelFileHandler.h"

ModelFileHandler::ModelFileHandler() {
	sectionTitles = {
		{FileSection::COLORS, "COLORS"},
		{FileSection::BLOCKS, "BLOCKS"},
		{FileSection::LIGHTS, "LIGHTS"},
		{FileSection::BOUNDS, "BOUNDS"},
		{FileSection::CAMERA, "CAMERA"}
	};

	subSectionTitles = {
		{FileSubSection::LIGHT_AMBIENT_COLOR, "LIGHT_AMBIENT_COLOR"},
		{FileSubSection::LIGHT_DIRECTIONAL, "LIGHT_DIRECTIONAL"},
		{FileSubSection::CAMERA_POSITION, "CAMERA_POSITION"},
		{FileSubSection::CAMERA_TARGET, "CAMERA_TARGET"}
	};
}