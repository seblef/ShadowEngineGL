
#include "VisibilityFactory.h"
#include "VisibilityAll.h"
#include "VisibilityQuad.h"

IVisibilitySystem* VisibilityFactory::createVisibilitySystem(const std::string& name)
{
	if(name=="quad")
		return new VisibilityQuad(8);
	else
		return new VisibilityAll;
}
