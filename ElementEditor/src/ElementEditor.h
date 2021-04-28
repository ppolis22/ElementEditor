#pragma once

#include "engine/RenderedApplication.h"

class ElementEditor : public RenderedApplication {
public:
	ElementEditor();
	~ElementEditor();

protected:
	void run(Window& window) override;
};