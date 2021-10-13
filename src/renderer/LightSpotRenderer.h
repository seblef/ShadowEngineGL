#pragma once

#include "LightRenderer.h"
#include "LightSpot.h"

class LightSpotRenderer : public LightRenderer
{
protected:

	bool				isCameraInVolume(const Vector3& eyePos, float minZ, const Vector3& pos, const Light *l) const;
	void				fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const;

public:

	LightSpotRenderer(const LightRendererCreate_t& c);

};
