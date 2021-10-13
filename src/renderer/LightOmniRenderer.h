#pragma once

#include "LightRenderer.h"

class LightOmniRenderer : public LightRenderer
{
protected:

	bool				isCameraInVolume(const Vector3& eyePos, float minZ, const Vector3& p, const Light *l) const;
	void				fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const;

public:

	LightOmniRenderer(const LightRendererCreate_t& c);

};
