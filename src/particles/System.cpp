#include "System.h"
#include "../core/YAMLCore.h"
#include "../loguru.hpp"
#include <algorithm>

namespace Particles
{

SystemTemplate::SystemTemplate(const YAML::Node& node)
{
    parse(node);
}

SystemTemplate::SystemTemplate(const std::string& filename)
{
	YAML::Node node;

	try
	{
		node = YAML::LoadFile(filename);
	}
	catch(const std::exception& e)
	{
		LOG_S(ERROR) << "Failed parsing particle system template " << filename << " - " << e.what();
		return;
	}
	parse(node);
}

SystemTemplate::SystemTemplate(const SystemTemplate& t)
{
    for(auto const& sub : t._subSystems)
        _subSystems.push_back(
            std::unique_ptr<SubSystemTemplate>(
                new SubSystemTemplate(*sub)
            )
        );
}

SystemTemplate::~SystemTemplate()
{
}

void SystemTemplate::parse(const YAML::Node& node)
{
	YAML::Node emitters = node["emitters"];
	for(YAML::const_iterator e=emitters.begin(); e!= emitters.end(); ++e)
		_subSystems.push_back(std::unique_ptr<SubSystemTemplate>(
            new SubSystemTemplate(
                e->first.as<std::string>(),
                e->second
            ))
        );
}

void SystemTemplate::addSubSystem(SubSystemTemplate* subSys)
{
    _subSystems.push_back(
        std::unique_ptr<SubSystemTemplate>(subSys)
    );
}

void SystemTemplate::remSubSystem(SubSystemTemplate* subSys)
{
    auto const& it = std::find_if(
        _subSystems.begin(),
        _subSystems.end(),
        [subSys](std::unique_ptr<SubSystemTemplate>& pt){ return pt.get() == subSys; }
    );
    if(it != _subSystems.end())
        _subSystems.erase(it);
}



System::System() :
    _worldBBox(Core::BBox3::BoundBox),
    _particlesCount(0)
{
}

System::System(const SystemTemplate& sys) :
    _worldBBox(Core::BBox3::BoundBox),
    _particlesCount(0)
{
    for(int i=0;i<sys.getSubCount();++i)
        _subSystems.push_back(
            std::unique_ptr<SubSystem>(
                new SubSystem(sys.getSubSystem(i))
            )
        );
}

System::~System()
{
}

void System::addSubSystem(SubSystem* subSys)
{
    _subSystems.push_back(
        std::unique_ptr<SubSystem>(subSys)
    );
}

void System::remSubSystem(SubSystem* subSys)
{
    auto const& it = std::find_if(
        _subSystems.begin(),
        _subSystems.end(),
        [subSys](std::unique_ptr<SubSystem>& pt){ return pt.get() == subSys; }
    );
    if(it != _subSystems.end())
        _subSystems.erase(it); 
}

SubSystem* System::getSubSystem(int i)
{
    return _subSystems[i].get();
}

void System::setWorldMatrix(const Core::Matrix4& worldMat)
{
    for(auto const& sub : _subSystems)
        sub->setWorldMatrix(worldMat);
}

void System::update(float time, const Core::Camera& camera)
{
	_worldBBox = Core::BBox3::InitBox;
	_particlesCount = 0;

    for(auto const& sub : _subSystems)
	{
		sub->update(time, camera);
		_worldBBox << sub->getWorldBBox();
		_particlesCount += sub->getParticlesCount();
	}
}

void System::collectParticles(
    Particle** p,
    unsigned int& count,
    unsigned int maxCount
)
{
    for(auto const& sub : _subSystems)
        sub->collectParticles(p, count, maxCount);
}

unsigned int System::countParticles()
{
    _particlesCount = 0;
    for(auto const& sub : _subSystems)
        _particlesCount += sub->getParticlesCount();
    return _particlesCount;
}

void System::reset()
{
    for(auto const& sub : _subSystems)
        sub->reset();
}

}