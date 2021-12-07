#pragma once

#include "Light.h"
#include "ShadowMapBuffer.h"
#include "../mediacommon/IVideoDevice.h"
#include <vector>

namespace Core
{
    class Camera;
}

struct LightRendererCreate_t
{
	IVideoDevice*			_device;
	IConstantBuffer*		_cLightBuffer;
	ShadowMapBuffer*		_shadowBuffer;
	RenderState				_rsIn;
	RenderState				_rsOut;
	DepthStencilState		_dsIn;
	DepthStencilState		_dsOut;

};

class LightRenderer
{
protected:

	struct LightBuffer_t
	{
		Color			_color;
		Vector4			_pos;
		Vector4			_dir;
		float			_nearAngle;
		float			_farAngle;
		float			_range;
		float			_invRange;
		Matrix4			_areaViewProj;
	};

	typedef std::vector<Light*>			LightVector;
	LightVector						_lights;

	IVideoDevice*					_device;
	IConstantBuffer*				_cBuffer;
	ShadowMapBuffer*				_shadowBuffer;
    IGeometryBuffer*                _geo;
	int								_indicesCount;
	IShader*						_noShadowShader;
	IShader*						_shadowShader;
	DepthStencilState				_dsIn;
	DepthStencilState				_dsOut;
	RenderState						_rsIn;
	RenderState						_rsOut;

	static bool						lightSort(const Light* l1, const Light* l2)
	{
		return (l1->getShadowMap() && l2->getShadowMap()==0) || (l1->getShadowMap()==0 && l2->getShadowMap());
	}

	void							renderLight(const Light* l, const Vector3& eyePos, float minZ);

	virtual bool					isCameraInVolume(const Vector3& eyePos, float minZ, const Vector3& pos, const Light* l) const=0;
	virtual void					fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const=0;

public:

	LightRenderer(const LightRendererCreate_t& c, const string& noShadowShader, const string& shadowShader);
	virtual ~LightRenderer();

	void							enqueueLight(Light *l);
	void							applyLights(const Camera& eyePos);
};
