#pragma once

#include "SubSystem.h"
#include <memory>
#include <string>
#include <vector>

namespace Core
{
class Camera;
}

namespace YAML
{
class Node;
}

namespace Particles
{

class SystemTemplate
{
protected:
    std::vector<std::unique_ptr<SubSystemTemplate>> _subSystems;

    void parse(const YAML::Node& node);

public:
    SystemTemplate() {}
    SystemTemplate(const YAML::Node& node);
    SystemTemplate(const std::string& filename);
    ~SystemTemplate();

    unsigned int getSubCount() const { return _subSystems.size(); }
    SubSystemTemplate* getSubSystem(int i) const { return _subSystems[i].get(); }

    void addSubSystem(SubSystemTemplate* subSys);
    void remSubSystem(SubSystemTemplate* subSys);
};

class System
{
protected:
    std::vector<std::unique_ptr<SubSystem>> _subSystems;
    Core::BBox3 _worldBBox;
    unsigned int _particlesCount;

public:
    System();
    System(const SystemTemplate& sys);
    ~System();

    void addSubSystem(SubSystem* subSys);
    void remSubSystem(SubSystem* subSys);
    SubSystem* getSubSystem(int i);
    int getSubCount() const { return _subSystems.size(); }

    void setWorldMatrix(const Core::Matrix4& worldMat);
    const Core::BBox3& getWorldBBox() const { return _worldBBox; }

    void update(float time, const Core::Camera& camera);

    void collectParticles(
        Particle** p,
        unsigned int& count,
        unsigned int maxCount
    );
    unsigned int getParticlesCount() const { return _particlesCount; }
    unsigned int countParticles();
};

}