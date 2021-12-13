#include "FilesStrings.h"
#include "../physic/PhysicDefs.h"
#include "../renderer/Light.h"
#include "../renderer/Material.h"
#include <map>
#include <string>


const std::map<std::string, PhysicShape> keyToShape = {
    { "box", PSHAPE_BOX },
    { "sphere", PSHAPE_SPHERE },
    { "capsule", PSHAPE_CAPSULE },
    { "mesh", PSHAPE_MESH},
    { "chull", PSHAPE_CONVEXHULL}
};
const std::map<PhysicShape, std::string> shapeToKey = {
    { PSHAPE_BOX, "box" },
    { PSHAPE_SPHERE, "sphere" },
    { PSHAPE_CAPSULE, "capsule" },
    { PSHAPE_MESH, "mesh" },
    { PSHAPE_CONVEXHULL, "chull" }
};


const std::map<std::string, TextureSlot> keyToSlot = {
    { "diffuse", TS_DIFFUSE },
    { "normal", TS_NORMALMAP },
    { "specular", TS_SPECULAR },
    { "emissive", TS_EMISSIVE},
    { "environment", TS_ENVIONMENT }
};
const std::map<TextureSlot, std::string> slotToKey = {
    { TS_DIFFUSE, "diffuse" },
    { TS_NORMALMAP, "normal" },
    { TS_SPECULAR, "specular" },
    { TS_EMISSIVE, "emissive" },
    { TS_ENVIONMENT, "environment" }
};

const std::map<std::string, BlendMode> keyToBlend = {
    { "zero", BLEND_ZERO },
    { "one", BLEND_ONE },
    { "src_color", BLEND_SRCCOLOR },
    { "inv_src_color", BLEND_INVSRCCOLOR },
    { "src_alpha", BLEND_SRCALPHA },
    { "inv_src_alpha", BLEND_INVSRCALPHA },
    { "dest_alpha", BLEND_DESTALPHA },
    { "inv_dest_alpha", BLEND_DESTALPHA },
    { "dest_color", BLEND_DESTCOLOR },
    { "inv_dest_color", BLEND_INVDESTCOLOR },
    { "factor", BLEND_FACTOR }
};
const std::map<BlendMode, std::string> blendToKey = {
    { BLEND_ZERO, "zero" },
    { BLEND_ONE, "one" },
    { BLEND_SRCCOLOR, "src_color" },
    { BLEND_INVSRCCOLOR, "inv_src_color" },
    { BLEND_SRCALPHA, "src_alpha" },
    { BLEND_INVSRCALPHA, "inv_src_alpha" },
    { BLEND_DESTALPHA, "dest_alpha" },
    { BLEND_INVDESTALPHA, "inv_dest_alpha" },
    { BLEND_DESTCOLOR, "dest_color" },
    { BLEND_INVDESTCOLOR, "inv_dest_color" },
    { BLEND_FACTOR, "factor" }    
};

const std::map<std::string, CullMode> keyToCull = {
    { "none", CULL_NONE },
    { "front", CULL_FRONT},
    { "back", CULL_BACK}
};
const std::map<CullMode, std::string> cullToKey = {
    { CULL_NONE, "none" },
    { CULL_FRONT, "front" },
    { CULL_BACK, "back" }
};

const std::map<std::string, Light::LightType> keyToLight = {
    { "omni", Light::LT_OMNI },
    { "spot", Light::LT_SPOT },
    { "area", Light::LT_AREA }
};
const std::map<Light::LightType, std::string> lightToKey = {
    { Light::LT_OMNI, "omni" },
    { Light::LT_SPOT, "spot" },
    { Light::LT_AREA, "area" }
};

const std::string emptyString("");


unsigned int getPhysicShapeFromKey(const std::string& name)
{
    auto const& shape = keyToShape.find(name);
    return shape == keyToShape.end() ? PSHAPE_COUNT : shape->second;
}
const std::string& getPhysicShapeKey(unsigned int shape)
{
    auto const& key = shapeToKey.find((PhysicShape)shape);
    return key == shapeToKey.end() ?  emptyString : key->second;
}

unsigned int getTextureSlotFromKey(const std::string& name)
{
    auto const& slot = keyToSlot.find(name);
    return slot == keyToSlot.end() ? TS_COUNT : slot->second;
}
const std::string& getTextureSlotKey(unsigned int slot)
{
    auto const& key = slotToKey.find((TextureSlot)slot);
    return key == slotToKey.end() ?  emptyString : key->second;
}

unsigned int getBlendModeFromKey(const std::string& name)
{
    auto const& blend = keyToBlend.find(name);
    return blend == keyToBlend.end() ? BLEND_COUNT : blend->second;
}
const std::string& getBlendModeKey(unsigned int mode)
{
    auto const& key = blendToKey.find((BlendMode)mode);
    return key == blendToKey.end() ?  emptyString : key->second;
}

unsigned int getCullModeFromKey(const std::string& name)
{
    auto const& cull = keyToCull.find(name);
    return cull == keyToCull.end() ? CULL_COUNT : cull->second;
}
const std::string& getCullModeKey(unsigned int mode)
{
    auto const& key = cullToKey.find((CullMode)mode);
    return key == cullToKey.end() ?  emptyString : key->second;
}

unsigned int getLightTypeFromKey(const std::string& name)
{
    auto const& type = keyToLight.find(name);
    return type == keyToLight.end() ? Light::LT_COUNT : type->second;
}
const std::string& getLightTypeKey(unsigned int type)
{
    auto const& key = lightToKey.find((Light::LightType)type);
    return key == lightToKey.end() ?  emptyString : key->second;
}
