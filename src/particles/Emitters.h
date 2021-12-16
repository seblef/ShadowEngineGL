#pragma once

#include "../core/Vector3.h"
#include <string>

namespace Particles
{

class Particle;

class IEmitter
{
private:
    std::string _class;

public:
    IEmitter(const std::string& eclass) : _class(eclass) {}
    virtual ~IEmitter() {}

    const std::string& getClass() const { return _class; }

    virtual void emit(Particle& particle, const Core::Matrix4& worldMat)=0;
};


class PointEmitter : public IEmitter
{
public:
    PointEmitter() : IEmitter("point") {}

    void emit(Particle& particle, const Core::Matrix4& worldMat);
};


class LineEmitter : public IEmitter
{
protected:
    Core::Vector3 _startPos;
    Core::Vector3 _endPos;

public:
    LineEmitter(
        const Core::Vector3& startPos = Core::Vector3::NullVector,
        const Core::Vector3& endPos = Core::Vector3::UnitVector
    );

    void setPositions(const Core::Vector3& startPos, const Core::Vector3& endPos);
    void emit(Particle& particle, const Core::Matrix4& worldMat);
};


class ExplosionEmitter : public IEmitter
{
protected:
    float _minSize;
    float _maxSize;

public:
    ExplosionEmitter(
        float minSize=1.f,
        float maxSize=5.f
    );

    void setSizes(float minSize, float maxSize);
    void emit(Particle& particle, const Core::Matrix4& worldMat);

};

}