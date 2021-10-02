#pragma once

#include "IR2D_FontLoader.h"

class R2D_StdFontLoader : public IR2D_FontLoader
{
public:

	R2D_StdFontLoader()				{}

	R2D_Font*						loadFont(const string& fontFile, IVideoDevice* device);
};
