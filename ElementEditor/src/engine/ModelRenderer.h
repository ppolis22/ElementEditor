#pragma once

#include "Renderable.h"
#include "Camera.h"

class ModelRenderer {
public:
	ModelRenderer();
	~ModelRenderer();

	void render(Renderable& renderable, Camera& camera);
};