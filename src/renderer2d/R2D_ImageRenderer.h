#pragma once

#include "R2D_Renderer.h"

class R2D_Object;
class ITexture;
class IVideoDevice;

class R2D_ImageRenderer : public R2D_Renderer
{
protected:

	static bool				imgSort(const R2D_Object* o1, const R2D_Object* o2);
	void					sortObjects();
	void					renderObjects();

public:

	R2D_ImageRenderer(bool relativePos, ITexture* white, IVideoDevice* dev);
};
