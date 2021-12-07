#pragma once

#include "Renderable.h"
#include "../core/Color.h"

class IShadowMap;

struct LightCreate_t
{
	Color		_color;
	float		_range;
	bool		_castShadows;
	int			_shadowMapSize;
	int			_shadowMapFilter;
	float		_farAngle;
	float		_nearAngle;
	float		_areaWidth;
	float		_areaHeight;
	float		_nearZ;
	Vector3		_direction;
	Matrix4		_world;
};

class Light : public Renderable
{
public:

	enum LightType
	{
		LT_OMNI=0,
		LT_SPOT,
		LT_AREA,

		LT_COUNT
	};

protected:

	LightType				_type;
	Color					_color;
	float					_range;
	bool					_castShadows;
	int						_shadowMapSize;
	int						_shadowMapFilter;
	Vector3					_direction;

	IShadowMap*				_shadowMap;
	Matrix4					_shadowViewProj;

	void					updateWorldMatrix();

public:

    static void				computeMatrix(const Vector3& d, Matrix4& m);

	Light(LightType type, const LightCreate_t& c);
	Light(const Light *l);
	virtual ~Light()																						{}

	LightType				getLightType() const						{ return _type; }
	float					getRange() const							{ return _range; }

	const Color&			getColor() const							{ return _color; }
	bool					getCastShadows() const						{ return _castShadows; }
	int						getShadowMapSize() const					{ return _shadowMapSize; }
	int						getShadowMapFilter() const					{ return _shadowMapFilter; }
	const Vector3&			getDirection() const						{ return _direction; }

	void					setRange(float r);
	void					setColor(const Color& c)					{ _color=c; }
	void					setCastShadows(bool castShadows)			{ _castShadows=castShadows; }
	void					setShadowMapSize(int size)					{ _shadowMapSize=size; }
	void					setShadowMapFilter(int f)					{ _shadowMapFilter=f; }
	void					setDirection(const Vector3& dir);

	void					enqueue();

	void					setShadowMap(IShadowMap* m)					{ _shadowMap=m; }
	void					setShadowViewProjMatrix(const Matrix4& vp)	{ _shadowViewProj=vp; }

	IShadowMap*				getShadowMap() const						{ return _shadowMap; }
	const Matrix4&			getShadowViewProjMatrix() const				{ return _shadowViewProj; }
};
