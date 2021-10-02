#pragma once

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
