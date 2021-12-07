
#pragma once

#include "../core/Vector3.h"

class FieldHit
{
public:

	FieldHit()			{}

	static void			hitField(const Core::Vector3& center, float innerRadius,
		float outerRadius, float innerDamage);
};
