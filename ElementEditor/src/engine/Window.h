#pragma once

#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "MouseListener.h"

class Window {
public:
	Window() = delete;
	Window(int width, int height, const char* title);
	~Window();

	bool isOpen();
	void makeCurrentContext();
	void swapBuffers();
	bool isKeyPressed(unsigned int keyCode);
	void setMouseCaptureMode(bool enable);
	void registerMouseListener(MouseListener& listener);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);		// could this be private/protected?

private:
	GLFWwindow* glfwWindow;
	int width;
	int height;

	static MouseListener* listener;
	static float lastCursorXPos;
	static float lastCursorYPos;
};