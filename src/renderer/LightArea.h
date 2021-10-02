#pragma once

#include "Light.h"

class LightArea : public Light
{
protected:

	float				_nearAngle;
	float				_farAngle;
	float				_areaWidth;
	float				_areaHeight;
	float				_nearZ;

public:

    LightArea(const LightCreate_t& c) : Light(LT_AREA,c), _nearAngle(c._nearAngle), _farAngle(c._farAngle),
        _areaWidth(c._areaWidth), _areaHeight(c._areaHeight), _nearZ(c._nearZ) {}
    LightArea(const LightArea* l) : Light(l), _nearAngle(l->_nearAngle), _farAngle(l->_farAngle),
        _areaWidth(l->_areaWidth), _areaHeight(l->_areaHeight), _nearZ(l->_nearZ)	{}

	float				getNearAngle() const				{ return _nearAngle; }
	float				getFarAngle() const					{ return _farAngle; }
	float				getAreaWidth() const				{ return _areaWidth; }
	float				getAreaHeight() const				{ return _areaHeight; }
	float				getNearZ() const					{ return _nearZ; }

	void				setNearAngle(float a)				{ _nearAngle=a; }
	void				setFarAngle(float a)				{ _farAngle=a; }
	void				setAreaWidth(float w)				{ _areaWidth=w; }
	void				setAreaHeight(float h)				{ _areaHeight=h; }
	void				setNearZ(float z)					{ _nearZ=z; }
};
