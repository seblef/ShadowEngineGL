#pragma once

#include "IResource.h"
#include <memory>

namespace Particles
{
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
    std::unique_ptr<Particles::SystemTemplate> _template;
    std::unique_ptr<Particles::SystemTemplate> _backup;

    void parse(const YAML::Node& node);

public:
    EdParticles();
    EdParticles(const std::string& filename);
    EdParticles(const std::string& name, const YAML::Node& node);
    ~EdParticles();

    Particles::SystemTemplate& getTemplate() const { return *_template; }

    void backup();
    void restore();
};

}