#include "Chunk.h"

Chunk::Chunk() { // Create the blocks
	data = new Block** [CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		data[i] = new Block* [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			data[i][j] = new Block[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() {
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] data[i][j];
		}
		delete[] data[i];
	}
	delete[] data;
}

void Chunk::render(Renderer renderer) {

}