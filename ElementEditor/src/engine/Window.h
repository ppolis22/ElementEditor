#pragma once

#include "RenderedApplication.h"	// TODO try replacing with glew/glfw includes and add Window.h include in RenderedApplication

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