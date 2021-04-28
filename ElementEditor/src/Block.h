#pragma once

enum BlockType : unsigned char {
	Empty = 0,
	Grass,
	Stone
};

class Block {
public:
	Block();
	~Block();

	BlockType getType();
private:
	BlockType type;
};