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
	data.rootUIElement = element;
}

UIElement* Window::getRootUIElement() {
	return data.rootUIElement;
}

static void updateUIElement(UIElement* element) {
	if (element == nullptr)
		return;

	element->update();
	for (UIElement* child : element->getChildren())
		updateUIElement(child);
}

void Window::updateUI() {
	updateUIElement(data.rootUIElement);
}

// handle events in children first, so event response is in reverse draw order
static void handleMouseMoveEvent(UIElement* element, MouseMoveEvent& event) {
	if (!element->isEnabled() || event.isHandled)
		return;

	for (UIElement* child : element->getChildren())
		handleMouseMoveEvent(child, event);

	if (!event.isHandled)
		element->processMouseMovement(event);
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	float deltaX = xpos - data.lastCursorXPos;
	float deltaY = ypos - data.lastCursorYPos;
	data.lastCursorXPos = xpos;
	data.lastCursorYPos = ypos;
	MouseMoveEvent event(xpos, ypos, deltaX, deltaY);
	handleMouseMoveEvent(data.rootUIElement, event);

	if (!event.isHandled) {
		data.applicationListener->processMouseMovement(event);
	}
}

static void handleKeyEvent(UIElement* element, KeyPressEvent& event) {
	if (!element->isEnabled() || event.isHandled)
		return;

	for (UIElement* child : element->getChildren())
		handleKeyEvent(child, event);

	if (!event.isHandled)
		element->processKeyPress(event);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) {
		KeyPressEvent event(key);
		handleKeyEvent(data.rootUIElement, event);

		if (!event.isHandled)
			data.applicationListener->processKeyPress(event);
	}
}

static void handleScrollEvent(UIElement* element, MouseScrollEvent& event) {
	if (!element->isEnabled() || event.isHandled)
		return;

	for (UIElement* child : element->getChildren())
		handleScrollEvent(child, event);

	if (!event.isHandled)
		element->processScroll(event);
}

void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	MouseScrollEvent event(yOffset);
	handleScrollEvent(data.rootUIElement, event);

	if (!event.isHandled)
		data.applicationListener->processScroll(event);
}

void Window::handleMouseDownEvent(UIElement* element, MouseButtonDownEvent& event, WindowData& data) {
	if (!element->isEnabled() || event.isHandled)
		return;

	for (UIElement* child : element->getChildren())
		handleMouseDownEvent(child, event, data);

	if (!event.isHandled) {
		element->processMouseDown(event);
		if (event.isHandled)
			data.preClickedUIElement = element;
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) {
		MouseButtonDownEvent event(button, data.lastCursorXPos, data.lastCursorYPos);
		handleMouseDownEvent(data.rootUIElement, event, data);
		if (!event.isHandled)
			data.applicationListener->processMouseDown(event);
	} else {
		MouseButtonUpEvent event(button, data.lastCursorXPos, data.lastCursorYPos);
		if (data.preClickedUIElement != nullptr) {
			data.preClickedUIElement->processMouseUp(event);
			data.preClickedUIElement = nullptr;
		} else {
			data.applicationListener->processMouseUp(event);
		}
	}
}