
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

using namespace Core;

Vector2 Vector2::NullVector(0,0);
Vector2 Vector2::XAxisVector(1,0);
Vector2 Vector2::YAxisVector(0,1);
Vector2 Vector2::UnitVector(1,1);

Vector3 Vector3::NullVector(0,0,0);
Vector3 Vector3::XAxisVector(1,0,0);
Vector3 Vector3::YAxisVector(0,1,0);
Vector3 Vector3::ZAxisVector(0,0,1);
Vector3 Vector3::UnitVector(1,1,1);

Vector4 Vector4::NullVector(0,0,0,0);
Vector4 Vector4::XAxisVector(1,0,0,0);
Vector4 Vector4::YAxisVector(0,1,0,0);
Vector4 Vector4::ZAxisVector(0,0,1,0);
Vector4 Vector4::UnitVector(1,1,1,1);

Vector4 Vector4::White(1,1,1,1);
Vector4 Vector4::Black(0,0,0,1);
Vector4 Vector4::Red(1,0,0,1);
Vector4 Vector4::Green(0,1,0,1);
Vector4 Vector4::Blue(0,0,1,1);


Vector2 Vector2::operator*(const Matrix2& m) const
{
    return Vector2(x*m(0,0) + y*m(1,0),
                    x*m(0,1) + y*m(1,1));
}

Vector2& Vector2::operator*=(const Matrix2& m)
{
    float tx=x;
    float ty=y;

    x=tx*m(0,0) + ty*m(1,0);
    y=tx*m(0,1) + ty*m(1,1);

    return *this;
}


Vector3 Vector3::operator*(const Matrix4& m) const
{
    return Vector3(x*m(0,0) + y*m(1,0) + z*m(2,0) + m(3,0),
                    x*m(0,1) + y*m(1,1) + z*m(2,1) + m(3,1),
                    x*m(0,2) + y*m(1,2) + z*m(2,2) + m(3,2));
}

Vector3& Vector3::operator*=(const Matrix4& m)
{
    float tx=x;
    float ty=y;
    float tz=z;

    x=tx*m(0,0) + ty*m(1,0) + tz*m(2,0) + m(3,0);
    y=tx*m(0,1) + ty*m(1,1) + tz*m(2,1) + m(3,1);
    z=tx*m(0,2) + ty*m(1,2) + tz*m(2,2) + m(3,2);

    return *this;
}

bool Vector3::isBetween(const Vector3& v1, const Vector3& v2) const
{
    float f=(v2-v1).getSquareLength();
    return (getDistanceFromSqr(v1) < f && getDistanceFromSqr(v2) < f);
}

void Vector3::transformNoTranslation(const Matrix4& m)
{
    float tx=x;
    float ty=y;
    float tz=z;

    x=tx*m(0,0) + ty*m(1,0) + tz*m(2,0);
    y=tx*m(0,1) + ty*m(1,1) + tz*m(2,1);
    z=tx*m(0,2) + ty*m(1,2) + tz*m(2,2);
}


Vector4 Vector4::operator*(const Matrix4& m) const
{
    return Vector4(x*m(0,0) + y*m(1,0) + z*m(2,0) + w*m(3,0),
                    x*m(0,1) + y*m(1,1) + z*m(2,1) + w*m(3,1),
                    x*m(0,2) + y*m(1,2) + z*m(2,2) + w*m(3,2),
                    x*m(0,3) + y*m(1,3) + z*m(2,3) + w*m(3,3));
}

Vector4& Vector4::operator*=(const Matrix4& m)
{
    float tx=x;
    float ty=y;
    float tz=z;
    float tw=w;

    x=tx*m(0,0) + ty*m(1,0) + tz*m(2,0) + tw*m(3,0);
    y=tx*m(0,1) + ty*m(1,1) + tz*m(2,1) + tw*m(3,1);
    z=tx*m(0,2) + ty*m(1,2) + tz*m(2,2) + tw*m(3,2);
    w=tx*m(0,3) + ty*m(1,3) + tz*m(2,3) + tw*m(3,3);

    return *this;
}

