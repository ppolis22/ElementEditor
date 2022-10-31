#pragma once

#include "Block.h"

#include "engine/geom/Point3d.h"
#include "engine/mesh/MeshBuilder3d.h"
#include "engine/mesh/Mesh.h"
#include "engine/render/Renderable.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

class ChunkManager;

class Chunk : public Renderable {
public:
	static const int CHUNK_SIZE = 4;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_BLOCK_WIDTH;

	struct Iterator {
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = BlockColor;
		using pointer = value_type*;
		using reference = value_type &;

		Iterator(std::vector<std::vector<std::vector<BlockColor>>>* data, int x, int y, int z) : data(data), x(x), y(y), z(z) {}

		reference operator*() const { return data->at(x).at(y).at(z); }
		pointer operator->() { return &(data->at(x).at(y).at(z)); }

		Iterator& operator++() { 
			z = (++z % Chunk::CHUNK_SIZE);
			if (z == 0) y = (++y % Chunk::CHUNK_SIZE);
			if (z == 0 && y == 0) x++;
			return *this; 
		}

		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		friend bool operator== (const Iterator& a, const Iterator& b) { return a.data == b.data && a.x == b.x && a.y == b.y && a.z == b.z; };
		friend bool operator!= (const Iterator& a, const Iterator& b) { return !(a == b); };

		int x, y, z;

	private:
		std::vector<std::vector<std::vector<BlockColor>>>* data;
	};

	Chunk() = delete;
	Chunk(int xPos, int yPos, int zPos, ChunkManager* manager);
	~Chunk();

	Iterator begin() { return Iterator(&data, 0, 0, 0); }
	Iterator end() { return Iterator(&data, CHUNK_SIZE, 0, 0); }

	void rebuildMesh();
	void unloadMesh();
	void setBlockColor(BlockColor color, Point3di location);
	void removeBlock(Point3di location);
	void addSelection(Point3di location);
	void removeSelection(Point3di location);
	void selectAll();
	void deselectAll();
	std::vector<Point3di> getSelection();
	BlockColor getBlockColor(Point3di location);
	Point3di getPosition();

	Mesh& getMesh() override;
	glm::mat4 getTransformation() override;

private:
	std::vector<std::vector<std::vector<BlockColor>>> data;
	std::vector <Point3di> selectedBlocks;
	MeshBuilder3d meshBuilder;
	Mesh mesh;
	int xPosition, yPosition, zPosition;

	// It may make more sense to have a static ChunkManager for the entire session, or a static/singleton
	// session object with a static/singleton ChunkManager. For now, this will do.
	ChunkManager* manager;

	void buildBlockMesh(int x, int y, int z, BlockColor color);
	bool blockIsEmpty(int x, int y, int z);
	bool blockIsEmpty(const Point3di& point);
	float calculateOcclusion(const Point3di& vertex, const Point3di& corner, const Point3df& normal);
};