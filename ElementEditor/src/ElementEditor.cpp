#include "ElementEditor.h"
#include "engine/Window.h"
#include "engine/Renderer.h"
#include "Chunk.h"

int main(void) {
	ElementEditor app;
	app.start();
}

ElementEditor::ElementEditor() {}

ElementEditor::~ElementEditor() {}

void ElementEditor::run(Window& window) {

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	Chunk chunk(0, 0, 0);
	chunk.setBlock(Stone, 0, 0, 0);
	chunk.setBlock(Stone, 1, 0, 0);
	chunk.setBlock(Stone, 0, 1, 0);
	chunk.rebuildMesh();
	Camera camera;
	Renderer renderer;

	while (window.isOpen()) {
		glClearColor(0.1f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.render(chunk, camera);

		window.swapBuffers();
		glfwPollEvents();
	}
}