#include "ALSoundVirtualSource.h"
#include <AL/al.h>


ALSoundVirtualSource::ALSoundVirtualSource() :
    _position(Vector3::NullVector),
    _velocity(Vector3::NullVector),
    _gain(1.f),
    _referenceDistance(2.f),
    _maxDistance(10.f),
    _rollOff(1.f),
    _inAngle(0.f),
    _outAngle(0.f),
    _relative(false),
    _playingSource(0)
{
}

ALSoundVirtualSource::ALSoundVirtualSource(
    const Vector3& pos,
    const Vector3& vel,
    const Vector3& dir,
    float refDist, float maxDist,
    float rollOff,
    float inAngle, float outAngle
) :
    _position(pos),
    _velocity(vel),
    _direction(dir),
    _referenceDistance(refDist),
    _maxDistance(maxDist),
    _rollOff(rollOff),
    _inAngle(inAngle),
    _outAngle(outAngle),
    _gain(1.f),
    _relative(false),
    _playingSource(0)
{
}

void ALSoundVirtualSource::setPosition(const Vector3& pos)
{
    _position = pos;
    if(_playingSource)
        alSource3f(_playingSource, AL_POSITION, pos.x, pos.y, pos.z);
}

void ALSoundVirtualSource::setVelocity(const Vector3& vel)
{
    _velocity = vel;
    if(_playingSource)
        alSource3f(_playingSource, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void ALSoundVirtualSource::setDirection(const Vector3& dir)
{
    _direction = dir;
    if(_playingSource)
        alSource3f(_playingSource, AL_DIRECTION, dir.x, dir.y, dir.z);
}

void ALSoundVirtualSource::setGain(float gain)
{
    _gain = gain;
    if(_playingSource)
        alSourcef(_playingSource, AL_GAIN, gain);
}

void ALSoundVirtualSource::setReferenceDistance(float refDist)
{
    _referenceDistance = refDist;
    if(_playingSource)
        alSourcef(_playingSource, AL_REFERENCE_DISTANCE, refDist);
}

void ALSoundVirtualSource::setRollOff(float rollOff)
{
    _rollOff = rollOff;
    if(_playingSource)
        alSourcef(_playingSource, AL_ROLLOFF_FACTOR, rollOff);
}

void ALSoundVirtualSource::setMaxDistance(float maxDist)
{
    _maxDistance = maxDist;
    if(_playingSource)
        alSourcef(_playingSource, AL_MAX_DISTANCE, maxDist);
}

void ALSoundVirtualSource::setRelative(bool relative)
{
    _relative = relative;
    if(_playingSource)
        alSourcei(_playingSource, AL_SOURCE_RELATIVE, (int)relative);
}

void ALSoundVirtualSource::setConeAngles(float inAngle, float outAngle)
{
    _inAngle = inAngle;
    _outAngle = outAngle;

    if(_playingSource)
    {
        alSourcef(_playingSource, AL_CONE_INNER_ANGLE, inAngle);
        alSourcef(_playingSource, AL_CONE_OUTER_ANGLE, outAngle);
    }
}

void ALSoundVirtualSource::setPlayingSource(unsigned int src)
{
    _playingSource = src;
    if(src == 0)
        return;
    
    alSource3f(_playingSource, AL_POSITION, _position.x, _position.y, _position.z);
    alSource3f(_playingSource, AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z);
    alSource3f(_playingSource, AL_DIRECTION, _direction.x, _direction.y, _direction.z);
    alSourcef(_playingSource, AL_GAIN, _gain);
    alSourcef(_playingSource, AL_REFERENCE_DISTANCE, _referenceDistance);
    alSourcef(_playingSource, AL_MAX_DISTANCE, _maxDistance);
    alSourcef(_playingSource, AL_ROLLOFF_FACTOR, _rollOff);
    alSourcef(_playingSource, AL_CONE_INNER_ANGLE, _inAngle);
    alSourcef(_playingSource, AL_CONE_OUTER_ANGLE, _outAngle);
    alSourcei(_playingSource, AL_SOURCE_RELATIVE, (int)_relative);
}