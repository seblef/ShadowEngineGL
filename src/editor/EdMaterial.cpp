#include "EdMaterial.h"
#include "../core/YAMLCore.h"
#include "../game/FilesStrings.h"
#include "../game/TextParser.h"
#include "../renderer/MaterialAnimationFactory.h"
#include "../renderer/MaterialFadeAnimation.h"
#include "../renderer/MaterialTextureAnimation.h"
#include "../renderer/MaterialUVAnimation.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"
#include <fstream>
#include <string.h>


namespace Editor
{

EdMaterial::EdMaterial() : 
    IResource(RES_MATERIAL, "Materials/Material"),
    _material(new Material(Renderer::getSingletonRef().getVideoDevice()))
{
}

EdMaterial::EdMaterial(const std::string& filename) :
    IResource(RES_MATERIAL, filename)
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
    IResource(RES_MATERIAL, name)
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

void EdMaterial::parse(const YAML::Node& node)
{
    _debrisMesh = node["debris"].as<string>("");

    YAML::Node r = node["renderer"];
	if(r)
    {
        _material = std::unique_ptr<Material>(parseRendererMaterial(r));
        _valid = true;
    }
    else
    {
        LOG_S(WARNING) << "No renderer section in material " << _name;
        _material = std::unique_ptr<Material>(new Material(Renderer::getSingletonRef().getVideoDevice()));
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

YAML::Node EdMaterial::buildYAMLNode() const
{
    IVideoDevice* device = Renderer::getSingletonRef().getVideoDevice();

    YAML::Node mat;
    mat["debris"] = _debrisMesh;

    YAML::Node renderer;

    YAML::Node textures;
    for(unsigned int i=0;i<TS_COUNT;++i)
    {
        if(!_material->isDefaultTexture((TextureSlot)i))
            textures[getTextureSlotKey(i)] = _material->getTexture((TextureSlot)i)->getName();
    }
    renderer["textures"] = textures;

    bool blendEnable;
    BlendMode src, dest;
    device->getBlendStateDesc(
        _material->getBlendState(),
        blendEnable,
        src, dest
    );
    if(blendEnable)
    {
        YAML::Node blend;
        blend["src"] = getBlendModeKey(src);
        blend["dest"] = getBlendModeKey(dest);
        renderer["blending"] = blend;
    }

    CullMode cull;
    FillMode fill;
    device->getRenderStateDesc(_material->getRenderState(), cull, fill);
    if(cull!=CULL_BACK)
        renderer["cull"] = getCullModeKey(cull);

    const Vector2& offset(_material->getStdBuffer().getOffset());
    const Vector2& scale(_material->getStdBuffer().getScale());
    if(offset!=Vector2::NullVector || scale!=Vector2::UnitVector)
    {
        YAML::Node uv;
        uv["offset"] = offset;
        uv["scale"] = scale;
        uv["rotation"] = .0f;
        renderer["uv"] = uv;
    }

    renderer["ambient_color"] = _material->getStdBuffer().getAmbient();
    renderer["diffuse_color"] = _material->getStdBuffer().getDiffuse();
    renderer["specular_color"] = _material->getStdBuffer().getSpecular();
    renderer["shininess"] = _material->getStdBuffer().getShininess();
    renderer["specular_intensity"] = _material->getStdBuffer().getSpecIntensity();
    renderer["reflectivity"] = _material->getStdBuffer().getReflection();

    if(_material->getFlag(MF_ALPHA))
        renderer["alpha"] = true;
    if(_material->getFlag(MF_CASTNOSHADOWS))
        renderer["cast_no_shadows"] = true;
    if(_material->getFlag(MF_ALPHASHADOWS))
        renderer["alpha_shadows"] = true;

    if(_material->getAnimationCount() > 0)
    {
        YAML::Node anims;
        for(int i=0;i<_material->getAnimationCount();++i)
        {
            MaterialAnimation* anim = _material->getAnimation(i);
            if(strcmp(anim->getAnimationName(), "uv") == 0)
            {
                YAML::Node uv;
                MaterialUVAnimation* uvanim = (MaterialUVAnimation*)anim;
                uv["offset"] = uvanim->getOffset();
                uv["rotation"] = uvanim->getRotation();
                anims["uv"] = uv;
            }
            else if(strcmp(anim->getAnimationName(), "texture") == 0)
            {
                YAML::Node tex;
                MaterialTextureAnimation* texanim = (MaterialTextureAnimation*)anim;
                tex["time"] = texanim->getAnimTime();
                tex["tile_count"] = texanim->getTileCount();
                tex["tiles_per_row"] = texanim->getTilePerRow();
                tex["tiles_per_col"] = texanim->getTilePerColumn();
                anims["texture"] = tex;
            }
            else if(strcmp(anim->getAnimationName(), "fade")==0)
            {
                YAML::Node fade;
                MaterialFadeAnimation* fadeanim = (MaterialFadeAnimation*)anim;
                fade["start_time"] = fadeanim->getStartTime();
                fade["end_time"] = fadeanim->getEndTime();
                anims["fade"] = fade;
            }
        }
        renderer["animations"] = anims;
    }

    mat["renderer"] = renderer;
    return mat;
}

bool EdMaterial::save(const std::string& filename) const
{
    std::ofstream mat_out(filename);
    if(!mat_out.is_open())
    {
        LOG_S(ERROR) << "Failed saving material " << _name << " in file " << filename;
        return false;
    }

    YAML::Node root;
    root["material"] = buildYAMLNode();

    mat_out << root;
    mat_out.close();
    LOG_S(INFO) << "Material " << _name << " has been exported in " << filename;
    return true;
}

}