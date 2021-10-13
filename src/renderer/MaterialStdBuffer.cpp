
#include "MaterialStdBuffer.h"


MaterialStdBuffer::MaterialStdBuffer() : 
    _ambient(Color::Black),
    _diffuse(Color::White),
    _specular(Color::Black),
    _UVOffset(Vector2::NullVector),
    _UVScale(Vector2::UnitVector),
    _UVRot(Matrix2::Identity),
    _shininess(10.f),
    _specIntensity(0.1f),
    _reflection(0.f)
{
}

MaterialStdBuffer::MaterialStdBuffer(const MaterialStdBuffer& m) :
    _ambient(m._ambient),
    _diffuse(m._diffuse),
    _specular(m._specular),
    _UVOffset(m._UVOffset),
    _UVScale(m._UVScale),
    _UVRot(m._UVRot),
    _shininess(m._shininess),
    _specIntensity(m._specIntensity),
    _reflection(m._reflection)
{
}

MaterialStdBuffer::MaterialStdBuffer(const MaterialCreate& mc) :
    _ambient(mc._ambient),
    _diffuse(mc._diffuse),
    _specular(mc._specular),
    _UVOffset(mc._uvOffset),
    _UVScale(mc._uvScale),
    _shininess(mc._shininess),
    _specIntensity(mc._specularIntensity),
    _reflection(mc._reflectivity)
{
    _UVRot.rotation(mc._uvRot * (float)M_PI / 180.0f);
}

MaterialStdBuffer&	MaterialStdBuffer::operator=(const MaterialStdBuffer& m)
{
    _ambient = m._ambient;
    _diffuse = m._diffuse;	
    _specular = m._specular;
    _shininess = m._shininess;
    _specIntensity = m._specIntensity;
    _reflection = m._reflection;
    _UVOffset = m._UVOffset;
    _UVScale = m._UVScale;
    _UVRot = m._UVRot;
    return *this;
}
