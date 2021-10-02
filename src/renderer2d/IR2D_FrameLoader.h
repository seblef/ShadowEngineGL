#pragma once

#include "R2D_Frame.h"

class IR2D_FrameLoader
{
public:

	IR2D_FrameLoader()				{}
	virtual ~IR2D_FrameLoader()		{}

	virtual R2D_Frame*				loadFrame(const string& frameFile, IVideoDevice* device)=0;
};
