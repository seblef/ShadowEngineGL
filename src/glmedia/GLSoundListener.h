#pragma once

#include "../MediaCommon.h"

class GLSoundListener : public ISoundListener
{
public:

    GLSoundListener()           {}
    ~GLSoundListener()          {}

    void			setPosition(const Vector3& pos)         {}
    void			setVelocity(const Vector3& vel)         {}
    void			setFrontAxis(const Vector3& f)          {}
    void			setTopAxis(const Vector3& t)            {}
    void			setCone(ISoundCone* c)                  {}

    const Vector3&	getPosition() const                     { return Vector3::NullVector; }
    const Vector3&	getVelocity() const                     { return Vector3::NullVector; }
    const Vector3&	getFrontAxis() const                    { return Vector3::ZAxisVector; }
    const Vector3&	getTopAxis() const                      { return Vector3::YAxisVector; }
    ISoundCone*		getCone() const                         { return 0; }
};
