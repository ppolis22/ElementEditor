#pragma once

class BlockColor {
public:
	unsigned char r, g, b, alpha = 255;		// NOTE: alpha currently supported only as boolean "is visible" flag

	inline bool isEmpty() { return alpha == 0; };
	inline float getNormalizedR() { return (float)r / 255.0f; };
	inline float getNormalizedG() { return (float)g / 255.0f; };
	inline float getNormalizedB() { return (float)b / 255.0f; };

	static BlockColor& EMPTY() { static BlockColor empty{ 0, 0, 0, 0 }; return empty; };
};

class Block {
public:
	BlockColor getColor();

private:
	BlockColor color;
};