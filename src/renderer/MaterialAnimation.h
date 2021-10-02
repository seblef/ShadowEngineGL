#pragma once

#include "Material.h"

class MaterialAnimation
{
protected:

	Material*				_material;

public:

	MaterialAnimation(Material *m) : _material(m)			{}
	virtual ~MaterialAnimation()							{}

	virtual const char*		getAnimationName() const=0;

	virtual void			set(float time)=0;
};
