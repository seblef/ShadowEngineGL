
#pragma once

#include "../renderer2d/R2D_Layer.h"
#include <string>


class R2D_Font;
class R2D_Text;


class FPSCounter
{
protected:

	double			_minFrameTime;
	double			_maxFrameTime;
	double			_totalFrameTime;
	double			_totalFrameCount;
	double			_lastTime;

	R2D_Font*		_font;
	R2D_Text*		_text;
	R2D_Layer*		_layer;

	std::string			_counterName;

public:

	FPSCounter(const std::string& counterName, bool showCounter, R2D_Font* fpsFont);
	~FPSCounter();

	R2D_Layer*		getLayer() const			{ return _layer; }

	void			updateCounter();
	void			outputStats();
};
