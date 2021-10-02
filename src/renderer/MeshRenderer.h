#pragma once

#include "TRenderer.h"
#include "MeshInstance.h"

class MeshRenderer : public TRenderer<MeshInstance>, public TSingleton<MeshRenderer>
{
protected:

	void					renderShadowAlpha(const ViewFrustum& vf);
public:

	MeshRenderer(IVideoDevice* device) :
		TRenderer<MeshInstance>(device,"Base/GBuffer","Base/Add","Base/Shadows","Base/ShadowsAlpha")		{}
	~MeshRenderer()													{}

	void					render(bool addPass);
	void					renderShadows(const ViewFrustum& vf);
};
