#pragma once

#include "MaterialCreate.h"

class MaterialStdBuffer
{
protected:

	Color			_ambient;
	Color			_diffuse;
	Color			_specular;

	Vector2			_UVOffset;
	Vector2			_UVScale;
	Matrix2			_UVRot;

	float _shininess;
	float _specIntensity;
	float _reflection;
	float _fpack;

public:

	MaterialStdBuffer();
    MaterialStdBuffer(const MaterialStdBuffer& m);
    MaterialStdBuffer(const MaterialCreate& mc);

	void			setAmbient(const Color& c)						{ _ambient = c; }
	void			setDiffuse(const Color& c)						{ _diffuse = c; }
	void			setSpecular(const Color& c)						{ _specular = c; }
	void			setShininess(float s)							{ _shininess = s; }
	void			setSpecIntensity(float i)						{ _specIntensity = i; }
	void			setReflection(float r)							{ _reflection = r; }
	
	void			setOffset(const Vector2& o)						{ _UVOffset=o; }
	void			setScale(const Vector2& s)						{ _UVScale=s; }
	void			setRotation(const Matrix2& r)					{ _UVRot=r; }

	const Color&	getAmbient() const								{ return _ambient; }
	const Color&	getDiffuse() const								{ return _diffuse; }
	const Color&	getSpecular() const								{ return _specular; }
	float			getShininess() const							{ return _shininess; }
	float			getSpecIntensity() const						{ return _specIntensity; }
	float			getReflection() const							{ return _reflection; }

	const Vector2&	getOffset() const								{ return _UVOffset; }
	const Vector2&	getScale() const								{ return _UVScale; }
	const Matrix2&	getRotation() const								{ return _UVRot; }

	MaterialStdBuffer&	operator=(const MaterialStdBuffer& m);

	operator const float*()	const									{ return (const float*)this; }
};
