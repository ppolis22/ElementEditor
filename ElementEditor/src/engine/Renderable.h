#pragma once

#include "Mesh.h"
#include "Shader.h"

class Renderable {
public:
	virtual Mesh& getMesh() = 0;
	virtual Shader& getShader() = 0;	// could provide default shader?
	virtual glm::mat4 getTransformation() = 0;	// could provide default "no transformation"
};