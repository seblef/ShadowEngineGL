#include "EdMaterial.h"
#include "../core/YAMLCore.h"
#include "../game/TextParser.h"
#include "../renderer/MaterialAnimationFactory.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"


namespace Editor
{

EdMaterial::EdMaterial() : 
    IResource(RES_MATERIAL, "Materials/Material"),
    _material(new Material(Renderer::getSingletonRef().getVideoDevice())),
    _valid(true)
{
}

EdMaterial::EdMaterial(const std::string& filename) :
    IResource(RES_MATERIAL, filename),
    _material(0),
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
    IResource(RES_MATERIAL, name),
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



EdMaterialSave::EdMaterialSave(Material* mat) :
    _material(mat),
    _buffer(mat->getStdBuffer()),
    _flags(mat->getFlag(MF_ALL))
{
	for(int i=0;i<TS_COUNT;++i)
	{
		if(!mat->isDefaultTexture((TextureSlot)i))
			_textures[i] = mat->getTexture((TextureSlot)i)->getName();
	}

	for(int i=0;i<mat->getAnimationCount();++i)
		_animations.getVector().push_back(MaterialAnimationFactory::copyAnimation(mat->getAnimation(i)));

    FillMode fm;
	Renderer::getSingletonRef().getVideoDevice()->getBlendStateDesc(mat->getBlendState(), _blendEnable, _srcBlend, _destBlend);
	Renderer::getSingletonRef().getVideoDevice()->getRenderStateDesc(mat->getRenderState(), _cull, fm);
}

void EdMaterialSave::restore()
{
	_material->getStdBuffer()=_buffer;

	_material->unsetFlag(MF_ALL);
	_material->setFlag(_flags);

	for(int i=0;i<TS_COUNT;++i)
	{
		TextureSlot ts = (TextureSlot)i;
		if((_material->isDefaultTexture(ts) &&
           !_textures[i].empty()) ||
		   (!_material->isDefaultTexture(ts) &&
           _textures[i]!=_material->getTexture(ts)->getName()))
        {
			_material->setTexture(ts, _textures[i]);
        }
	}

	IVideoDevice *device = Renderer::getSingletonRef().getVideoDevice();
	device->destroyBlendState(_material->getBlendState());
	device->destroyRenderState(_material->getRenderState());

	_material->setBlendState(device->createBlendState(_blendEnable, _srcBlend, _destBlend));
	_material->setRenderState(device->createRenderState(_cull, FILL_SOLID));

	while(_material->getAnimationCount() > 0)
		_material->deleteAnimation(0);

	for(int i=0;i<_animations.getVector().size();++i)
		_material->addAnimation(MaterialAnimationFactory::copyAnimation(_animations.getVector()[i]));
}

}