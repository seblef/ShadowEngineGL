#pragma once

#include "TRenderer.h"
#include "ActorInstance.h"

class ActorSkinnedRenderer : public TRenderer<ActorInstance>, public TSingleton<ActorSkinnedRenderer>
{
protected:

	IConstantBuffer*		_boneCBuffer;

	void					renderAlphaShadows(const ViewFrustum& vf);

public:

	ActorSkinnedRenderer(IVideoDevice* device);
	~ActorSkinnedRenderer();

	void					render(bool addPass);
	void					renderShadows(const ViewFrustum& vf);
};
