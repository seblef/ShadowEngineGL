
#pragma once

#include "../R2D.h"

class HUDCross
{
protected:

	R2D_Image			*_cross;

public:

	HUDCross(R2D_Layer* hudLayer);
	~HUDCross()							{}

	void				updateCross();
};
