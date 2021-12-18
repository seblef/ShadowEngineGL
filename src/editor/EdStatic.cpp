#include "EdStatic.h"

namespace Editor
{

EdStaticTemplate::EdStaticTemplate(const std::string& name) :
    IResource(RES_STATIC, name)
{
}

EdStaticTemplate::EdStaticTemplate(
    const std::string& name,
    const YAML::Node& node
) :
    IResource(RES_STATIC, name),
    EdSolidTemplate(node)
{
    _valid = true;
}


StaticObject::StaticObject(EdStaticTemplate* stat) :
    SolidObject(OBJ_STATIC, stat)
{
}

StaticObject::StaticObject(const StaticObject& stat) :
    SolidObject(stat)
{
}

}