#include "UIControl.h"

UIControl::UIControl(float x, float y, float width, float height)
	: UIElement(x, y, width, height), isEnabled(true) {}

bool UIControl::isEnabled() {
	return isEnabled;
}

void UIControl::setEnabled(bool enabled) {
	isEnabled = enabled;
}

void UIControl::addListener(ActionListner* listener) {
	listeners.push_back(listener);
}

void UIControl::alertListeners() {
	for (ActionListner* listener : listeners) {
		listener->actionPerformed({ this });
	}
}