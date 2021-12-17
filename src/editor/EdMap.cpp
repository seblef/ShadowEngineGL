#include "EdMap.h"
#include "EdGround.h"
#include "EdMaterial.h"
#include "EdStatic.h"
#include "Resources.h"
#include "../core/FileSystemFactory.h"
#include "../core/YAMLCore.h"
#include "../game/Ground.h"
#include "../game/GroundFileFormat.h"
#include "../renderer/LightSystem.h"
#include "../renderer/MaterialSystem.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"
#include <memory>

namespace Editor
{

EdMap::EdMap(const std::string& filename) :
    _ground(0),
    _width(0),
    _height(0),
    _startPosition(Core::Vector2::NullVector),
    _valid(false)
{
    YAML::Node root;
    try
    {
        root = YAML::LoadFile(filename);
    }
    catch(const std::exception& e)
    {
        LOG_S(ERROR) << "Failed loading map " << filename << " - " << e.what();
        return;
    }

    YAML::Node mapNode = root["map"];
    parseMapNode(mapNode);

    YAML::Node globalLightNode = root["global_light"];
    if(globalLightNode)
        parseGlobalLightNode(globalLightNode);
    else
        LightSystem::getSingletonRef().getGlobalLight().disableGlobalLight();

    YAML::Node resources = root["resources"];
    parseResourcesNode(resources);

    loadGround();
}

EdMap::~EdMap()
{
    if(_ground)
        delete _ground;
}

void EdMap::parseMapNode(const YAML::Node& node)
{
    _width = node["width"].as<int>();
    _height = node["height"].as<int>();
    _startPosition =node["start_position"].as<Core::Vector2>();
    _groundFile = node["ground"].as<string>();
    Core::Color ambientLight = node["ambient_light"].as<Core::Color>();

    LightSystem::getSingletonRef().getGlobalLight().setAmbientLight(ambientLight);
}

void EdMap::parseGlobalLightNode(const YAML::Node& node)
{
    Core::Vector3 dir = node["direction"].as<Core::Vector3>();
    Core::Color color = node["color"].as<Core::Color>();

    LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(
        color, dir
    );
}

void EdMap::parseResourcesNode(const YAML::Node& node)
{
    YAML::Node materials = node["materials"];
    parseMaterialsNode(materials);

    YAML::Node geometries = node["geometries"];
    parseGeometriesNode(geometries);

    YAML::Node statics = node["meshes"];
    parseStaticsNode(statics);
}

void EdMap::parseMaterialsNode(const YAML::Node& node)
{
    for(YAML::const_iterator mat = node.begin(); mat!=node.end(); ++mat)
    {
        const std::string& name(mat->first.as<std::string>());
        EdMaterial* material = new EdMaterial(
            name,
            mat->second
        );
        if(material->isValid())
            Resources::getSingletonRef().add(RES_MATERIAL, material, name);
    }
}

void EdMap::parseGeometriesNode(const YAML::Node& node)
{
    for(auto geo=node.begin(); geo!=node.end(); ++geo)
        Resources::getSingletonRef().load(RES_GEOMETRY, geo->as<std::string>());
}

void EdMap::parseStaticsNode(const YAML::Node& node)
{
    for(auto stat=node.begin(); stat!=node.end(); ++stat)
    {
        const std::string& name(stat->first.as<string>());
        EdStaticTemplate* statTemplate = new EdStaticTemplate(
            name, stat->second
        );
        Resources::getSingletonRef().add(
            RES_STATIC,
            statTemplate,
            name
        );
    }
}

void EdMap::loadGround()
{
    std::unique_ptr<Core::IFile> f(
        Core::FileSystemFactory::getFileSystem()->openFile(_groundFile, Core::FA_READ)
    );
	File::GroundFileHeaderEx h;
	File::GroundFileMaterial m;
	f->read(&h, sizeof(File::GroundFileHeader));

    if(h._header != GROUND_HEADER)
    {
        LOG_S(ERROR) << "Wrong header in map file";
        return;
    }
    if(h._width != _width || h._height != _height)
    {
        LOG_S(ERROR) << "Mismatch between ground and map sizes";
        return;
    }

	unsigned int* flags = new unsigned int[h._width*h._height];
    EdMaterial** mats = new EdMaterial*[h._matCount];
	std::unique_ptr<Material*[]> mm(new Material*[h._matCount]);
    std::unique_ptr<int[]> matIds(new int[_width * _height]);

	for(int i=0;i<h._matCount;++i)
	{
		f->read(&m,sizeof(File::GroundFileMaterial));
		mats[i] = (EdMaterial*)Resources::getSingletonRef().get(RES_MATERIAL, m._name);
        mm[i] = mats[i]->getMaterial();
	}

	for (int i = 0; i < h._gameMatCount; ++i)
		f->read(&m, sizeof(File::GroundFileMaterial));

	f->read(flags, sizeof(unsigned int) * h._width * h._height);

    unsigned int *flag_ptr = flags;
    int *mat_ptr = matIds.get();
    for(int i=0;i<_width * _height;++i, ++flag_ptr, ++mat_ptr)
        *mat_ptr = ((int)(*flag_ptr & GMATERIAL_MASK)) - 1;

    _ground = new EdGround(
        _width,
        _height,
        h._matCount,
        mats,
        mm.get(),
        flags,
        matIds.get()
    );
}

}
