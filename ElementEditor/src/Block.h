#pragma once

class BlockColor {
public:
	unsigned char r, g, b, alpha = 255;		// NOTE: alpha currently supported only as boolean "is visible" flag

	inline bool isEmpty() { return alpha == 0; };
	static BlockColor& EMPTY() { static BlockColor empty{ 0, 0, 0, 0 }; return empty; };
};

class Block {
public:
	BlockColor getColor();

private:
	BlockColor color;
};