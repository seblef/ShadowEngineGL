#pragma once

#include "../core/TSingleton.h"
#include <set>


class IUpdatable;


class UpdateSystem : public Core::TSingleton<UpdateSystem>
{
protected:

	typedef std::set<IUpdatable*>	UpdatableSet;
	UpdatableSet				_updatables;

public:

	UpdateSystem()				{}

	void						registerUpdatable(IUpdatable* u);
	void						unregisterUpdatable(IUpdatable* u);
	void						process(float time) const;
};
