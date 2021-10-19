#include "SelectableState.h"
#include "../AppController.h"

SelectableState::SelectableState(AppController* context)
	: BaseEditorState(context),
	rayTracer(context->getWindow()->getWidth(), context->getWindow()->getHeight(), context->getCamera()->getProjectionMatrix(), 25.0f) {}