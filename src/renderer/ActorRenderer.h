#pragma once

#include "TRenderer.h"
#include "ActorInstance.h"
#include "../core/TSingleton.h"


class ActorRenderer : public TRenderer<ActorInstance>, public Core::TSingleton<ActorRenderer>
{
protected:

	void					renderAlphaShadows(const ViewFrustum& vf);

public:

	ActorRenderer(IVideoDevice* device) : TRenderer<ActorInstance>(device,"Base/GBuffer","Base/Add","Base/Shadows","Base/ShadowsAlpha")			{}
	~ActorRenderer()													{}

	void					render(bool addPass);
	void					renderShadows(const ViewFrustum& vf);
};
