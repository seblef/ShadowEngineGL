#include "MatrixHierarchy.h"
#include <memory.h>

using namespace Core;

MatrixHierarchy::MatrixHierarchy(int count, bool offsets) :
    TTab<Matrix4>(count),
    _offsets(0),
    _offsetsEnable(0)
{
    _parents=new int[count];
    for(int i=0;i<count;++i)
        _parents[i]=-1;

    if (offsets)
    {
        _offsets = new Matrix4[count];
        _offsetsEnable = new bool[count];

        for (int i = 0; i < count; ++i)
            _offsetsEnable[i] = false;
    }
}

MatrixHierarchy::MatrixHierarchy(const MatrixHierarchy& mh) :
    TTab<Matrix4>(*((const TTab<Matrix4>*)&mh)),
    _offsets(0),
    _offsetsEnable(0)
{
    _parents=new int[mh.getCount()];
    for(int i=0;i<mh.getCount();++i)
        _parents[i]=mh._parents[i];

    if (mh._offsets)
    {
        _offsets = new Matrix4[mh.getCount()];
        _offsetsEnable = new bool[mh.getCount()];

        memcpy(_offsets, mh._offsets, sizeof(Matrix4)* mh.getCount());
        memcpy(_offsetsEnable, mh._offsetsEnable, sizeof(bool)* mh.getCount());
    }
}

MatrixHierarchy::~MatrixHierarchy()
{
    delete[] _parents;
    if (_offsets)
    {
        delete[] _offsets;
        delete[] _offsetsEnable;
    }
}

int MatrixHierarchy::getParent(int n) const
{
    return _parents[n];
}

void MatrixHierarchy::setParent(int n, int p)
{
    _parents[n]=p;
}

int* MatrixHierarchy::getParents() const
{
    return _parents;
}

void MatrixHierarchy::enableOffset(int idx, bool enable)
{
    _offsetsEnable[idx] = enable;
}

void MatrixHierarchy::setOffset(int idx, const Matrix4& offset)
{
    _offsets[idx] = offset;
}

void MatrixHierarchy::update(const Matrix4& world, const TTab<Matrix4>& pose)
{
    if (_offsets)
    {
        _tab[0] = pose.getItem(0)*world;
        for (int i = 1; i < _count; ++i)
        {
            _tab[i] = pose.getItem(i) * _tab[_parents[i]];
            if (_offsetsEnable[i])
                _tab[i] = _offsets[i] * _tab[i];
        }
    }
    else
    {
        _tab[0] = pose.getItem(0)*world;
        for (int i = 1; i<_count; ++i)
            _tab[i] = pose.getItem(i) * _tab[_parents[i]];
    }
}