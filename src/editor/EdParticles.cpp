#include "EdParticles.h"
#include "../particles/System.h"

namespace Editor
{

EdParticles::EdParticles() : 
    IResource(RES_PARTICLES),
    _template(new Particles::SystemTemplate)
{
}

EdParticles::EdParticles(const std::string& filename) :
    IResource(RES_PARTICLES, filename),
    _template(new Particles::SystemTemplate(filename + ".yaml"))
{
}

EdParticles::EdParticles(const std::string& name, const YAML::Node& node) :
    IResource(RES_PARTICLES, name),
    _template(new Particles::SystemTemplate(node))
{
}

EdParticles::~EdParticles()
{
}

void EdParticles::backup()
{
    _backup = std::unique_ptr<Particles::SystemTemplate>(
        new Particles::SystemTemplate(*_template)
    );
}

void EdParticles::restore()
{
    if(_backup)
        _template = std::move(_backup);
}

}
