#pragma once

#include "IResource.h"

namespace Particles
{
class SubSystemTemplate;
class SystemTemplate;
}

namespace YAML
{
class Node;
}

namespace Editor
{

class EdParticles : public IResource
{
protected:
    Particles::SystemTemplate *_template;
    Particles::SystemTemplate *_backup;

    void parse(const YAML::Node& node);
    YAML::Node buildSubSystemNode(const Particles::SubSystemTemplate& sub) const;

public:
    EdParticles();
    EdParticles(const std::string& filename);
    EdParticles(const std::string& name, const YAML::Node& node);
    ~EdParticles();

    Particles::SystemTemplate& getTemplate() const { return *_template; }

    void backup();
    void restore();

    bool save(const std::string& filename) const;
    YAML::Node buildYAMLNode() const;
};

}