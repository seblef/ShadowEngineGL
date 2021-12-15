#include "Emitters.h"
#include "Particle.h"
#include "../core/Matrix4.h"
#include "../core/Random.h"

namespace Particles
{


const float ParticleExplosionVelocityFactor = 2.5f;


void PointEmitter::emit(Particle& p, const Core::Matrix4& worldMat)
{
    p._pos = worldMat;
}


LineEmitter::LineEmitter(
    const Core::Vector3& startPos,
    const Core::Vector3& endPos
) :
    IEmitter("line"),
    _startPos(startPos),
    _endPos(endPos)
{
}

void LineEmitter::setPositions(const Core::Vector3& startPos, const Core::Vector3& endPos)
{
    _startPos = startPos;
    _endPos = endPos;
}

void LineEmitter::emit(Particle& p, const Core::Matrix4& worldMat)
{
    const Core::Vector3 worldStartPos(_startPos * worldMat);
    const Core::Vector3 worldEndPos(_endPos * worldMat);
    const Core::Vector3 delta(worldEndPos - worldStartPos);
    float t = RAND;

    p._pos = worldStartPos + delta * t;
}


ExplosionEmitter::ExplosionEmitter(float minSize, float maxSize) :
    IEmitter("explosion"),
    _minSize(minSize),
    _maxSize(maxSize)
{
}

void ExplosionEmitter::setSizes(float minSize, float maxSize)
{
    _minSize = minSize;
    _maxSize = maxSize;
}

void ExplosionEmitter::emit(Particle& p, const Core::Matrix4& worldMat)
{
    float rot_xy = RAND * 2.0f * M_PI;
    float rot_xz = RAND * 2.0f * M_PI;
    float c_rot_xz = cosf(rot_xz);
    float s_rot_xz = sinf(rot_xz);
    float c_rot_xy = cosf(rot_xy);
    float s_rot_xy = sinf(rot_xy);

    p._vel.x = s_rot_xy * c_rot_xz;
    p._vel.y = c_rot_xy * c_rot_xz;
    p._vel.z = s_rot_xz;
    p._vel *= ParticleExplosionVelocityFactor;

    p._pos = worldMat;

    float maxSize = RAND * _maxSize * 2.0f + 2.0f;
	Core::randomCounterVector(
        p._sizeCounter,
        p._size,
        Core::Vector3(_minSize, _minSize, _minSize),
        Core::Vector3(_maxSize, _maxSize, _maxSize),
        p._life
    );
}

}