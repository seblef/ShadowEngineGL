#pragma once

#include "../core/Matrix4.h"
#include "../core/Vector2.h"

class IConstantBuffer;
class IShadowMap;
class IVideoDevice;

using namespace Core;

class ShadowMapBuffer
{
protected:

	Matrix4					_viewProj;
	Vector2					_invShadowMapSize;
	float					_shadowFilter;
	float					_invShadowFilter;

	IConstantBuffer*		_cBuffer;

public:

	ShadowMapBuffer(IVideoDevice* device);
	~ShadowMapBuffer();

    void					set();
	void					setupLight(IShadowMap *m, int filter, const Matrix4& viewProj);
};
