
#ifndef _BUILDINGLIGHT_H_
#define _BUILDINGLIGHT_H_

#include "../StdRenderer.h"

#define BLIGHT_INTERIOR				(1 << 0)

class BuildingLight
{
protected:

	Light::LightType		_type;
	LightCreate_t			_light;
	unsigned int			_flags;

public:

	BuildingLight(Light::LightType type, const LightCreate_t& lc, unsigned int flags) : _type(type), _flags(flags)
	{
		memcpy(&_light, &lc, sizeof(LightCreate_t));
	}
	~BuildingLight()		{}

	Light::LightType		getLightType() const			{ return _type; }
	const LightCreate_t&	getLightDesc() const			{ return _light; }
	unsigned int			getFlags() const				{ return _flags; }
};

#endif
