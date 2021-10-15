#pragma once

#include "../MediaCommon.h"

class ALSoundListener : public ISoundListener
{
protected:

    Vector3 _position;
    Vector3 _velocity;
    Vector3 _frontAxis;
    Vector3 _topAxis;
    float _gain;

    void updateOrientiation() const;

public:

    ALSoundListener();
    ~ALSoundListener()          {}

    void			setPosition(const Vector3& pos);
    void			setVelocity(const Vector3& vel);
    void			setFrontAxis(const Vector3& f);
    void			setTopAxis(const Vector3& t);
    void            setGain(float gain);

    const Vector3&	getPosition() const                     { return _position; }
    const Vector3&	getVelocity() const                     { return _velocity; }
    const Vector3&	getFrontAxis() const                    { return _frontAxis; }
    const Vector3&	getTopAxis() const                      { return _topAxis; }
    float getGain() const                                   { return _gain; }
};
