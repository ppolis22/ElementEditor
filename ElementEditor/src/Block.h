#pragma once

struct BlockColor {
	unsigned char r, g, b;
	bool empty = true;		// replace with alpha
};

class Block {
public:
	BlockColor getColor();

private:
	BlockColor color;
};