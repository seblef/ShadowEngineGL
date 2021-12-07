#include "MaterialFadeAnimation.h"
#include "MaterialTextureAnimation.h"
#include "MaterialUVAnimation.h"


void MaterialFadeAnimation::set(float time)
{
    Color c(_material->getStdBuffer().getDiffuse());
    c.a=0;
    if(time <= _start)				c.a=1;
    else if(time < _end)			c.a=(time - _start) / (_end - _start);

    _material->getStdBuffer().setDiffuse(c);
}


void MaterialTextureAnimation::set(float time)
{
	int idx=((int)(time* ((float)_tileCount) / _animTime)) % _tileCount;
	int row_idx=idx / _tilePerRow;
	int col_idx=idx % _tilePerRow;

	Vector2 s(1 / ((float)_tilePerRow),1 / ((float)_tilePerColumn));

	_material->getStdBuffer().setScale(s);
	_material->getStdBuffer().setOffset(Vector2(s.x * (float)row_idx,s.y * (float)col_idx));
}

void MaterialUVAnimation::set(float time)
{
    _material->getStdBuffer().setOffset(_offsetPerSec * time);
    Matrix2 m;
    m.rotation(_rotPerSec * time * (float)M_PI / 180.0f);
    _material->getStdBuffer().setRotation(m);
}
