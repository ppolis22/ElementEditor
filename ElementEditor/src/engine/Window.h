#pragma once

#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>

class Window {
public:
	Window() = delete;
	Window(int width, int height, const char* title);
	~Window();

	bool isOpen();
	void makeCurrentContext();
	void swapBuffers();
	// TODO methods to get input

private:
	GLFWwindow* glfwWindow;
};