#pragma once

class ElementEditor;

class EditorState {
public:
	virtual ~EditorState();

	void setContext(ElementEditor* context);
	virtual void processLeftClick(float posX, float posY) = 0;
	virtual void render() = 0;

protected:
	ElementEditor* context;
};