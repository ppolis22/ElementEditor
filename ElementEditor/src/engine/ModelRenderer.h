#pragma once

#include "Renderable.h"
#include "Camera.h"

class ModelRenderer {
public:
	ModelRenderer();
	~ModelRenderer();

	void render(Renderable& renderable, Camera& camera);
	void renderPreview(Renderable& renderable, Camera& camera);

private:
	void renderWithTransparency(Renderable& renderable, Camera& camera, float alpha);
};