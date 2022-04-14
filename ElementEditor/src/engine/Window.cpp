#include "Window.h"
#include "ElementEngineException.h"

Window::Window(int width, int height, const char* title) : width(width), height(height) {
	glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!glfwWindow) {
		throw ElementEngineException("Could not create GLFW window.");
	}

	data.lastCursorXPos = width / 2.0f;
	data.lastCursorYPos = height / 2.0f;
	glfwMakeContextCurrent(glfwWindow);
	glfwSetWindowUserPointer(glfwWindow, &data);

	glfwSetCursorPosCallback(glfwWindow, mouse_callback);
	glfwSetKeyCallback(glfwWindow, key_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);
	glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
}

Window::~Window() {}

void Window::update() {
	glfwPollEvents();		// could be glfwWaitEvents() ?
	glfwSwapBuffers(glfwWindow);
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

bool Window::isOpen() {
	return !glfwWindowShouldClose(glfwWindow);
}

void Window::makeCurrentContext() {
	glfwMakeContextCurrent(glfwWindow);
}

bool Window::isKeyPressed(int keyCode) {
	return glfwGetKey(glfwWindow, keyCode) == GLFW_PRESS;
}

bool Window::isClicked(int mouseButtonCode) {
	return glfwGetMouseButton(glfwWindow, mouseButtonCode);
}

void Window::setApplicationEventListener(EventListener* listener) {
	data.applicationListener = listener;
}

void Window::setRootUIElement(UIElement* element) {
	data.uiListeners.clear();
	registerEventListener(element);
	this->rootUIElement = element;
}

UIElement* Window::getRootUIElement() {
	return rootUIElement;
}

static void updateUIElement(UIElement* element) {
	if (element == nullptr) {
		return;
	}

	element->update();
	for (UIElement* child : element->getChildren()) {
		updateUIElement(child);
	}
}

void Window::updateUI() {
	updateUIElement(rootUIElement);
}

void Window::registerEventListener(UIElement* listener) {
	// register children as event listeners first, so event response is in reverse draw order
	if (listener != nullptr) {
		for (UIElement* child : listener->getChildren()) {
			registerEventListener(child);
		}
		data.uiListeners.push_back(listener);
	}
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	float deltaX = xpos - data.lastCursorXPos;
	float deltaY = ypos - data.lastCursorYPos;
	data.lastCursorXPos = xpos;
	data.lastCursorYPos = ypos;
	MouseMoveEvent event(xpos, ypos, deltaX, deltaY);
	for (EventListener* listener : data.uiListeners) {
		listener->processMouseMovement(event);
		if (event.isHandled) return;
	}

	if (!event.isHandled) {
		data.applicationListener->processMouseMovement(event);
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) {
		KeyPressEvent event(key);
		for (EventListener* listener : data.uiListeners) {
			listener->processKeyPress(event);
			if (event.isHandled) return;
		}

		if (!event.isHandled) {
			data.applicationListener->processKeyPress(event);
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	MouseScrollEvent event(yOffset);
	for (EventListener* listener : data.uiListeners) {
		listener->processScroll(event);
		if (event.isHandled) return;
	}

	data.applicationListener->processScroll(event);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) {
		MouseButtonDownEvent event(button, data.lastCursorXPos, data.lastCursorYPos);
		for (EventListener* listener : data.uiListeners) {
			listener->processMouseDown(event);
			if (event.isHandled) return;
		}
		data.applicationListener->processMouseDown(event);
	} else {
		MouseButtonUpEvent event(button, data.lastCursorXPos, data.lastCursorYPos);
		for (EventListener* listener : data.uiListeners) {
			listener->processMouseUp(event);
			if (event.isHandled) return;
		}
		data.applicationListener->processMouseUp(event);
	}
}