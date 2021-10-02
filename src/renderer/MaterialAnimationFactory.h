#pragma once

#include "MaterialAnimation.h"

class MaterialAnimationFactory
{
public:

	MaterialAnimationFactory()			{}

	static MaterialAnimation*			createAnimation(const string& animName, Material *m);
	static MaterialAnimation*			copyAnimation(const MaterialAnimation* a);
};
