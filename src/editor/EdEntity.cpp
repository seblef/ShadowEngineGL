#include "EdEntity.h"
#include <map>

namespace Editor
{

const std::map<std::string, EntityClass> keyToEntity = {
    { "dynamic", ENTITY_DYNAMIC },
    { "explosive", ENTITY_EXPLOSIVE },
    { "trigger", ENTITY_TRIGGER}
};
const std::map<EntityClass, std::string> entityToKey = {
    { ENTITY_DYNAMIC, "dynamic" },
    { ENTITY_EXPLOSIVE, "explosive" },
    { ENTITY_TRIGGER, "trigger" }
};

const std::map<std::string, TriggerClass> keyToTrigger = {
    { "weapon", TRIGGER_WEAPON }
};
const std::map<TriggerClass, std::string> triggerToKey = {
    { TRIGGER_WEAPON, "weapon" }
};

const std::string emptyString("");

EntityClass getEntityClassFromKey(const std::string& key)
{
    auto const& eclass = keyToEntity.find(key);
    return eclass == keyToEntity.end() ? ENTITY_COUNT : eclass->second;
}
const std::string& getKeyFromEntityClass(EntityClass eclass)
{
    auto const& ekey = entityToKey.find(eclass);
    return ekey == entityToKey.end() ? emptyString : ekey->second;
}

TriggerClass getTriggerClassFromKey(const std::string& key)
{
    auto const& tclass = keyToTrigger.find(key);
    return tclass == keyToTrigger.end() ? TRIGGER_COUNT : tclass->second;
}
const std::string& getKeyFromTriggerClass(TriggerClass tclass)
{
    auto const& tkey = triggerToKey.find(tclass);
    return tkey == triggerToKey.end() ? emptyString : tkey->second;
}


EdEntityTemplate::EdEntityTemplate(EntityClass eclass) :
    _class(eclass),
    _density(1.f),
    _damageThreshold(20.f),
    _animated(false),
    _noRebirth(false)
{
}

EdEntityTemplate::EdEntityTemplate(const YAML::Node& node) :
    _density(1.f),
    _damageThreshold(20.f),
    _animated(false),
    _noRebirth(false)
{

}

}