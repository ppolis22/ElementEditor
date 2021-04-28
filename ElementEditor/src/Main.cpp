#include <GL/glew.h>		// Must be included before any other OpenGL stuff
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewInit();
	std::cout << glGetString(GL_VERSION) << std::endl;

	{	// new scope to ensure that stack-allocated objects with OpenGl calls in their destructors get destroyed before glfwTerminate()
		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}