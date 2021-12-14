
#pragma once

#include "Action.h"

class RParticles;
class TraceTemplate;

namespace Core
{
    class Vector3;
}


class Trace : public Action
{
protected:

	RParticles*				_particleSystem;

public:

	Trace(const TraceTemplate& t, const Core::Vector3& from, const Core::Vector3& to);
	~Trace();

	void					update(float time);
};
