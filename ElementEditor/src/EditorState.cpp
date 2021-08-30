#include "EditorState.h"

EditorState::~EditorState() {}

void EditorState::setContext(ElementEditor* context) {
	this->context = context;
}