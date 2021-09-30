
#include "MaterialTextureAnimation.h"


void MaterialTextureAnimation::set(float time)
{
	int idx=((int)(time* ((float)_tileCount) / _animTime)) % _tileCount;
	int row_idx=idx / _tilePerRow;
	int col_idx=idx % _tilePerRow;

	Vector2 s(1 / ((float)_tilePerRow),1 / ((float)_tilePerColumn));

	_material->getStdBuffer().setScale(s);
	_material->getStdBuffer().setOffset(Vector2(s.x * (float)row_idx,s.y * (float)col_idx));
}
