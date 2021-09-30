
#include "FPSCounter.h"
#include "GameSystem.h"
#include "../loguru.hpp"

FPSCounter::FPSCounter(const string& counterName, bool showCounter, R2D_Font* fpsFont) :
	_counterName(counterName), _minFrameTime(1e6), _maxFrameTime(0), _totalFrameTime(0),
	_totalFrameCount(0), _lastTime(0), _font(fpsFont), _layer(0), _text(0)
{
	if (showCounter)
	{
		_layer = new R2D_Layer;
		_text = new R2D_Text(fpsFont, "FPS:", Vector2(0.1f, 0.1f), Vector2::UnitVector, Color(0.8f, 0.8f, 0.8f, 0.6f));
		_layer->getSet().insert(_text);
	}
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::updateCounter()
{
	double currentTime=GameSystem::getSingletonRef().getGameTime();
	double frameTime = currentTime - _lastTime;
	_lastTime = currentTime;

	if (frameTime > 0)
	{
		if (frameTime < _minFrameTime)
			_minFrameTime = frameTime;

		if (frameTime > _maxFrameTime)
			_maxFrameTime = frameTime;

		_totalFrameTime += frameTime;
		_totalFrameCount += 1.0;

		_text->setText("FPS: " + to_string(1.0 / frameTime));
	}
}

void FPSCounter::outputStats()
{
	double meanFrameTime = _totalFrameTime / _totalFrameCount;
	LOG_SCOPE_F(INFO, "<----------------- FPS Counter %s ----------------->", _counterName);
	LOG_S(INFO) << "Mean frame time:	" << meanFrameTime;
	LOG_S(INFO) << "Min frame time:		" << _minFrameTime;
	LOG_S(INFO) << "Max frame time:		" << _maxFrameTime;
	LOG_S(INFO) << "Mean FPS:			" << 1.0 / meanFrameTime;
	LOG_S(INFO) << "Max FPS:			" << 1.0 / _minFrameTime;
	LOG_S(INFO) << "Min FPS:			" << 1.0 / _maxFrameTime;
}
