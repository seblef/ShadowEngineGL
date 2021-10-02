#pragma once

#include "R2D_Renderer.h"

class R2D_LineRenderer : public R2D_Renderer
{
protected:

	void					renderObjects();

public:

	R2D_LineRenderer(bool relativePos, ITexture* white, IVideoDevice* dev) : R2D_Renderer(R2D_Object::R2D_LINE,relativePos,white,dev)			{}
};
