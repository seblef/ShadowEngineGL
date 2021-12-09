#include "EdMaterial.h"
#include "../core/YAMLCore.h"
#include "../game/TextParser.h"
#include "../renderer/Material.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"


namespace Editor
{

EdMaterial::EdMaterial() : 
    IResource(RES_MATERIAL),
    _name("Materials/Material"),
    _material(new Material(Renderer::getSingletonRef().getVideoDevice())),
    _valid(true)
{
}

EdMaterial::EdMaterial(const std::string& filename) :
    IResource(RES_MATERIAL),
    _material(0),
    _name(filename),
    _valid(false)
{
    YAML::Node root;
    try
    {
        root = YAML::LoadFile(filename + ".yaml");
        YAML::Node mat_node(root["material"]);
        parse(mat_node);
    }
    catch(const std::exception& e)
    {
        LOG_S(ERROR) << "Failed loading material " << filename << ": " << e.what();
    }
    
}

EdMaterial::EdMaterial(const std::string& name, const YAML::Node& node) :
    IResource(RES_MATERIAL),
    _name(name),
    _material(0),
    _valid(false)
{
    try
    {
        parse(node);
    }
    catch(const std::exception& e)
    {
        LOG_S(ERROR) << "Failed parsing material " << name << ": " << e.what();
    }
    
}

EdMaterial::~EdMaterial()
{
    if(_material)
        delete _material;
}

void EdMaterial::parse(const YAML::Node& node)
{
    _debrisMesh = node["debris"].as<string>("");

    YAML::Node r = node["renderer"];
	if(r)
    {
        _material = parseRendererMaterial(r);
        _valid = true;
    }
    else
    {
        LOG_S(WARNING) << "No renderer section in material " << _name;
        _material = new Material(Renderer::getSingletonRef().getVideoDevice());
    }
}

}