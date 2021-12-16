#include "EdParticles.h"
#include "../core/YAMLCore.h"
#include "../game/FilesStrings.h"
#include "../mediacommon/ITexture.h"
#include "../particles/Material.h"
#include "../particles/System.h"
#include "../particles/TextureSet.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"


namespace Editor
{

EdParticles::EdParticles() : 
    IResource(RES_PARTICLES),
    _template(new Particles::SystemTemplate),
    _backup(0)
{
}

EdParticles::EdParticles(const std::string& filename) :
    IResource(RES_PARTICLES, filename),
    _template(new Particles::SystemTemplate(filename + ".yaml")),
    _backup(0)
{
}

EdParticles::EdParticles(const std::string& name, const YAML::Node& node) :
    IResource(RES_PARTICLES, name),
    _template(new Particles::SystemTemplate(node)),
    _backup(0)
{
}

EdParticles::~EdParticles()
{
    if(_backup)
        delete _backup;
    if(_template)
        delete _template;
}

void EdParticles::backup()
{
    if(_backup)
        delete _backup;
    _backup = new Particles::SystemTemplate(*_template);
}

void EdParticles::restore()
{
    if(_backup)
    {
        if(_template)
            delete _template;
        _template = _backup;
        _backup = 0;
        backup();
    }
}

YAML::Node EdParticles::buildYAMLNode() const
{
    YAML::Node emitters;
    for(int i=0;i<_template->getSubCount();++i)
    {
        const Particles::SubSystemTemplate& sub(*_template->getSubSystem(i));
        emitters[sub.getName()] = buildSubSystemNode(sub);
    }

    YAML::Node root;
    root["emitters"] = emitters;
    return root;
}

YAML::Node EdParticles::buildSubSystemNode(const Particles::SubSystemTemplate& sub) const
{
    YAML::Node subNode;
    subNode["renderer"] = sub.getRenderer();
    subNode["emitter"] = sub.getEmitter();
    subNode["max_particles"] = sub.getMaxParticles();
    subNode["emission_rate"] = sub.getEmissionRate();
    subNode["gravity"] = sub.getGravity();

    const Particles::SubSystemParams& pmin(sub.getMinParams());
    const Particles::SubSystemParams& pmax(sub.getMaxParams());

    subNode["life"].push_back(pmin.life);
    subNode["life"].push_back(pmax.life);

    subNode["mass"].push_back(pmin.mass);
    subNode["mass"].push_back(pmax.mass);

    subNode["friction"].push_back(pmin.friction);
    subNode["friction"].push_back(pmax.friction);

    subNode["velocity"].push_back(pmin.velocity);
    subNode["velocity"].push_back(pmax.velocity);

    subNode["start_color"].push_back(pmin.startColor);
    subNode["start_color"].push_back(pmax.startColor);

    subNode["end_color"].push_back(pmin.endColor);
    subNode["end_color"].push_back(pmax.endColor);

    subNode["start_size"].push_back(pmin.startSize);
    subNode["start_size"].push_back(pmax.startSize);

    subNode["end_size"].push_back(pmin.endSize);
    subNode["end_size"].push_back(pmax.endSize);

    YAML::Node matNode;
    const Particles::Material& mat(*sub.getMaterial());
    if(mat.getTexture())
        matNode["texture"] = mat.getTexture()->getName();
    else if(mat.getTextureSet())
        matNode["texture_dir"] = mat.getTextureSet()->getFolder();

    IVideoDevice* device = Renderer::getSingletonRef().getVideoDevice();
    bool blendEnabled;
    BlendMode srcBlend, destBlend;
    device->getBlendStateDesc(
        mat.getBlendState(),
        blendEnabled,
        srcBlend, destBlend
    );
    if(blendEnabled)
    {
        YAML::Node blendNode;
        blendNode["src"] = getBlendModeKey(srcBlend);
        blendNode["dest"] = getBlendModeKey(destBlend);
        matNode["blend"] = blendNode;
    }
    subNode["material"] = matNode;

    return subNode;
}

bool EdParticles::save(const std::string& filename) const
{
    std::ofstream p_out(filename);
    if(!p_out.is_open())
    {
        LOG_S(ERROR) << "Failed saving particles system " << _name << " in file " << filename;
        return false;
    }

    p_out << buildYAMLNode();
    LOG_S(INFO) << "Particles system " << _name << " has been exported in " << filename;
    return true;

}

}
