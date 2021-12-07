
#pragma once

#include "../renderer2d/R2D_Layer.h"

class R2D_Image;

class HUDCross
{
protected:

	R2D_Image			*_cross;

public:

	HUDCross(R2D_Layer* hudLayer);
	~HUDCross()							{}

	void				updateCross();
};
