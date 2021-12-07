
#pragma once

namespace Core
{
    class BBox2;
    class BBox3;
    class SRect;
    class Vector2;
}

using namespace Core;

void computeRect(const BBox3& b, SRect& r);
void computeRect(const BBox3& b, SRect& r, const Vector2& offset);
void computeRect(const BBox2& b, SRect& r);
void computeRect(const BBox2& b, SRect& r, const Vector2& offset);
