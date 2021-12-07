#include "R2D_Image.h"
#include "../mediacommon/ITexture.h"

R2D_Image::R2D_Image(bool autoDeleteTexture) :
    R2D_Object(R2D_IMG),
    _autoDeleteTexture(autoDeleteTexture),
    _texture(0),
    _uvStart(Vector2::NullVector),
    _uvSize(Vector2::UnitVector)
{
}

R2D_Image::R2D_Image(
    const Vector2& pos,
    const Vector2& size,
    const Color& c,
    const Vector2& uvStart,
    const Vector2& uvSize,
    ITexture *t,
    bool autoDeleteTexture
) :
    R2D_Object(R2D_IMG,pos,size,c),
    _autoDeleteTexture(autoDeleteTexture),
    _texture(t),
    _uvStart(uvStart),
    _uvSize(uvSize)
{
}

R2D_Image::~R2D_Image()
{
    if(_autoDeleteTexture && _texture)
        _texture->remRef();
}
