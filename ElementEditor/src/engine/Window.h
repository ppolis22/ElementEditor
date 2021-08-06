#pragma once

#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

#include "InputListener.h"

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
	void registerInputListener(InputListener& listener);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);		// could these be private/protected?
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	GLFWwindow* glfwWindow;
	int width;
	int height;

	static InputListener* listener;
	static float lastCursorXPos;
	static float lastCursorYPos;
};