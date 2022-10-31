#pragma once

#include "Shader.h"
#include "engine/mesh/Mesh.h"

class Renderable {
public:
	virtual Mesh& getMesh() = 0;
	virtual glm::mat4 getTransformation() = 0;	// could provide default "no transformation"
};