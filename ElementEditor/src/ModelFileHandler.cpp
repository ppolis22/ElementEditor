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
		{FileSubSection::LIGHT_DIRECTIONAL_COLOR, "LIGHT_DIRECTIONAL_COLOR"},
		{FileSubSection::LIGHT_DIRECTIONAL_POSITION, "LIGHT_DIRECTIONAL_POSITION"},
		{FileSubSection::LIGHT_DIRECTIONAL_TARGET, "LIGHT_DIRECTIONAL_TARGET"},
		{FileSubSection::CAMERA_POSITION, "CAMERA_POSITION"},
		{FileSubSection::CAMERA_TARGET, "CAMERA_TARGET"}
	};
}