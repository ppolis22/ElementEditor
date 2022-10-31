#pragma once

#include <string>

class Texture {

public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline std::string getPath() { return filePath; }

private:
	unsigned int rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, bpp;
};