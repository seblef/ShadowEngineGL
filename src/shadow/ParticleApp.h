
#ifndef _PARTICLEAPP_H_
#define _PARTICLEAPP_H_

#include "../Core.h"

using namespace Core;

class ParticleApp
{
protected:

	void					updateCamera(Camera& c, const Vector2& rot, float dist) const;

public:

	ParticleApp()			{}

    void					run(const std::string& dataFolder);
};

#endif
