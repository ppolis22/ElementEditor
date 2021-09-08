#pragma once

#include "engine/RenderedApplication.h"
#include "engine/Camera.h"
#include "engine/ModelRenderer.h"
#include "ChunkManager.h"
#include "BaseEditorState.h"

class ElementEditor : public RenderedApplication {
public:
	ElementEditor();
	~ElementEditor();

protected:
	void run() override;

};