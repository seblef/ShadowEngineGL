#pragma once

#include "R2D_Frame.h"

class R2D_FrameLoader
{
public:

	R2D_FrameLoader()				{}

	static R2D_Frame*				loadFrame(const string& frameFile, IVideoDevice* device);
};
