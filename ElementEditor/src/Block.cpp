#include "Block.h"

BlockColor::BlockColor()
	: BlockColor(0, 0, 0, 0) {}

BlockColor::BlockColor(unsigned char r, unsigned char g, unsigned char b)
	: BlockColor(r, g, b, 255) {}

BlockColor::BlockColor(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha)
	: r(r), g(g), b(b), alpha(alpha) {}

BlockColor::BlockColor(glm::vec3 color)
	: r((unsigned char)(color.r * 255)), 
	g((unsigned char)(color.g * 255)), 
	b((unsigned char)(color.b * 255)), 
	alpha(255) {}

BlockColor Block::getColor() {
	return color;
}