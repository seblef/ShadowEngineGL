
#pragma once

#include "EntityTemplate.h"

class GameEntityTrigger;
class Trigger;


class EntityTrigger : public EntityTemplate
{
protected:

	Trigger*			_trigger;
	bool				_animated;

public:

	EntityTrigger(const YAML::Node& node);
	~EntityTrigger();

	void				load();
	void				unload();

	bool				isAnimated() const					{ return _animated; }

	Trigger*			getTrigger() const					{ return _trigger; }

	GameEntity*			createInstance(const Matrix4& world);
};
