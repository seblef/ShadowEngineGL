
#pragma once

#include "Action.h"
#include "../core/Color.h"
#include "../core/Vector3.h"

class Light;

using namespace Core;

#define FLASH_ASCENDING				0
#define FLASH_STABLE				1
#define FLASH_DESCENDING			2

class Flash : public Action
{
protected:

	float			_durations[3];
	int				_state;
	float			_time;
	Color			_color;

	Light*			_rLight;

public:

	Flash(const Color& c, float ascendDuration, float stableDuration,
		float descendDuration, float range, const Vector3& position);
	~Flash();

	void			update(float time);
};
