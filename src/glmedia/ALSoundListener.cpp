#include "ALSoundListener.h"
#include <AL/al.h>


ALSoundListener::ALSoundListener() : 
    _position(Vector3::NullVector),
    _velocity(Vector3::NullVector),
    _frontAxis(Vector3::ZAxisVector),
    _topAxis(Vector3::YAxisVector),
    _gain(1.0f)
{
}

void ALSoundListener::setPosition(const Vector3& pos)
{
    _position = pos;
    alListenerfv(AL_POSITION, (const float*)&pos);
}

void ALSoundListener::setVelocity(const Vector3& vel)
{
    _velocity = vel;
    alListenerfv(AL_VELOCITY, (const float*)&vel);
}

void ALSoundListener::setTopAxis(const Vector3& topAxis)
{
    _topAxis = topAxis;
    updateOrientiation();
}

void ALSoundListener::setFrontAxis(const Vector3& frontAxis)
{
    _frontAxis = frontAxis;
    updateOrientiation();
}

void ALSoundListener::updateOrientiation() const
{
    float orientation[6] = {
        _frontAxis.x,
        _frontAxis.y,
        _frontAxis.z,
        _topAxis.x,
        _topAxis.y,
        _topAxis.z
    };
    alListenerfv(AL_ORIENTATION, orientation);
}

void ALSoundListener::setGain(float gain)
{
    _gain = gain;
    alListenerf(AL_GAIN, gain);
}
