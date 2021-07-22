#include "ElementEditor.h"
#include "engine/Window.h"
#include "Chunk.h"

int main(void) {
	ElementEditor app;
	app.start();
}

ElementEditor::ElementEditor() {}

ElementEditor::~ElementEditor() {}

void ElementEditor::run(Window& window) {

	MeshBuilder meshBuilder;
	Chunk chunk(0, 0, 0);
	chunk.setBlock(Stone, 0, 0, 0);
	chunk.rebuildMesh(meshBuilder);

	while (window.isOpen()) {
		glClearColor(0.1f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		window.swapBuffers();
		glfwPollEvents();
	}
}