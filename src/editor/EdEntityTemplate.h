#pragma once

#include "EdSolidTemplate.h"

namespace YAML
{
class Node;
}

namespace Editor
{

enum EntityClass
{
    ENTITY_DYNAMIC = 0,
    ENTITY_EXPLOSIVE,
    ENTITY_TRIGGER,

    ENTITY_COUNT
};

enum TriggerClass
{
    TRIGGER_WEAPON = 0,

    TRIGGER_COUNT
};

class EdEntityTemplate : public EdSolidTemplate
{
protected:
    EntityClass _class;

    // Dynamic entity properties
    float _density;

    // Explosive entity properties
    std::string _explosion;
    float _damageThreshold;

    // Trigger properties
    bool _animated;
    TriggerClass _triggerClass;

    // Weapon trigger properties
    std::string _weapon;
    bool _noRebirth;

public:

    EdEntityTemplate(EntityClass eclass);
    EdEntityTemplate(const YAML::Node& node);

    const EntityClass getClass() const { return _class; }

    void getDynamicProperties(float& density) const;
    void setDynamicProperties(float density);

    void getExplosiveProperties(std::string& explosion, float& damageThreshold) const;
    void setExplosiveProperties(const std::string& explosion, float damageThreshold);

    void getTriggerProperties(TriggerClass& tclass, bool& animated) const;
    void setTriggerProperties(TriggerClass tclass, bool animated);

    void getWeaponTriggerProperties(std::string& weapon, bool& noRebirth) const;
    void setWeaponTriggerProperties(const std::string& weapon, bool noRebirth);
};

EntityClass getEntityClassFromKey(const std::string& key);
const std::string& getKeyFromEntityClass(EntityClass eclass);

TriggerClass getTriggerClassFromKey(const std::string& key);
const std::string& getKeyFromTriggerClass(TriggerClass tclass);

}