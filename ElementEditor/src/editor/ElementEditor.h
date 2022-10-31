#pragma once

#include "editor/controller/ChunkManager.h"

#include "engine/app/RenderedApplication.h"
#include "engine/render/Camera.h"
#include "engine/render/ModelRenderer.h"

class ElementEditor : public RenderedApplication {
public:
	ElementEditor();
	~ElementEditor();

protected:
	void run() override;

};