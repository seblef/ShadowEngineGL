
#pragma once

#include "EntityTemplate.h"

class EntityDynamic : public EntityTemplate
{
protected:

	float					_density;

public:

	EntityDynamic(const YAML::Node& node);

	virtual void			load();
	virtual void			unload();

	virtual GameEntity*		createInstance(const Matrix4& world);

	float					getDensity() const					{ return _density; }
};
