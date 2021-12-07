#pragma once

#include "../core/Matrix4.h"
#include "../core/Vector4.h"
#include "../core/Vector2.h"

namespace Core
{
    class Camera;
}

using namespace Core;

class SceneInfosBuffer
{
protected:

	Matrix4				_viewProj;
	Matrix4				_invViewProj;
	Matrix4				_invProj;
	Matrix4				_proj;
	Matrix4				_view;
	Vector4				_eyePos;
	Vector2				_invScreenSize;
	float				_time;
	float				_pack;

public:

	SceneInfosBuffer() : _viewProj(Matrix4::Identity), _invViewProj(Matrix4::Identity), _invProj(Matrix4::Identity),
		_proj(Matrix4::Identity), _view(Matrix4::Identity), _eyePos(Vector4::NullVector),
		_invScreenSize(Vector2::UnitVector), _time(0), _pack(0)																	{}

	void				setCamera(const Camera& c);
	void				setTime(float time)						{ _time=time; }
};
