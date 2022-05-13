#pragma once

#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "EventListener.h"
#include "ui/UIElement.h"
#include <vector>

class Window {
public:
	Window() = delete;
	Window(int width, int height, const char* title);
	~Window();

	int getWidth();
	int getHeight();
	bool isOpen();
	void makeCurrentContext();
	bool isKeyPressed(int keyCode);
	bool isClicked(int mouseButtonCode);
	void update();
	void setApplicationEventListener(EventListener* listener);
	void setRootUIElement(UIElement* element);
	UIElement* getRootUIElement();
	void updateUI();	// TODO add filter to specify which elements to update

private:
	GLFWwindow* glfwWindow;
	int width;
	int height;

	struct WindowData {
		float lastCursorXPos;
		float lastCursorYPos;
		UIElement* rootUIElement;
		EventListener* applicationListener;
	};

	WindowData data;

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};