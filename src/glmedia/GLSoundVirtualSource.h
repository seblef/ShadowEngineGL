#pragma once

#include "../MediaCommon.h"

class GLSoundVirtualSource : public ISoundVirtualSource
{
public:

    GLSoundVirtualSource()              {}
    ~GLSoundVirtualSource()             {}

    void		setPosition(const Vector3& pos)         {}
    void		setVelocity(const Vector3& vel)         {}
    void		setInRadius(float r)                    {}
    void		setInRadiusAngle(float a)               {}
    void		setCone(ISoundCone* c)                  {}
    void		setFrontAxis(const Vector3& f)          {}
    void		setTopAxis(const Vector3& t)            {}
    void		setDopplerScale(float d)                {}
    void		setRelative(bool relative)              {}
    void		setVolume(float volume)                 {}

    const Vector3&		getPosition() const             { return Vector3::NullVector; }
    const Vector3&		getVelocity() const             { return Vector3::NullVector; }
    float				getInRadius() const             { return 1.0f; }
    float				getInRadiusAngle() const        { return 1.0f; }
    ISoundCone*			getCone() const                 { return 0; }
    const Vector3&		getFrontAxis() const            { return Vector3::ZAxisVector; }
    const Vector3&		getTopAxis() const              { return Vector3::YAxisVector; }
    float				getDopplerScale() const         { return 0.0f; }
    bool				isRelative() const              { return false; }
    float				getVolume() const               { return 1.0f; }
};
