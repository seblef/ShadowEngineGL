#pragma once

#include <string>


unsigned int getPhysicShapeFromKey(const std::string& name);
unsigned int getTextureSlotFromKey(const std::string& name);
unsigned int getBlendModeFromKey(const std::string& name);
unsigned int getCullModeFromKey(const std::string& name);
unsigned int getLightTypeFromKey(const std::string& name);


const std::string& getPhysicShapeKey(unsigned int shape);
const std::string& getTextureSlotKey(unsigned int slot);
const std::string& getBlendModeKey(unsigned int mode);
const std::string& getCullModeKey(unsigned int mode);
const std::string& getLightTypeKey(unsigned int type);
