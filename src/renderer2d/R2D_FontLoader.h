#pragma once

#include <string>

class R2D_Font;
class IVideoDevice;

class R2D_FontLoader
{
public:

	R2D_FontLoader()			{}

	static R2D_Font*			loadFont(const std::string& fontFile, IVideoDevice* device);
};
