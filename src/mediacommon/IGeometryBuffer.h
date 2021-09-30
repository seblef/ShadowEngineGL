#ifndef IGEOMETRYBUFFER_H
#define IGEOMETRYBUFFER_H

#include "VideoDefs.h"
#include "../Core.h"

using namespace Core;

class IGeometryBuffer
{
protected:

    int				_vxCount;
    VertexType		_vxType;
    int             _idxCount;
    IndexType		_idxType;
    BufferUsage		_usage;

public:

    IGeometryBuffer(int vxCount, VertexType vxType,
                    int indCount, IndexType idxType,
                    BufferUsage bu) : _vxCount(vxCount), _vxType(vxType), _idxCount(indCount), _idxType(idxType), _usage(bu)    {}
    virtual ~IGeometryBuffer()              {}

    virtual void    set()=0;
    virtual void    fillVertices(const void* data, int count=0, int offset=0)=0;
    virtual void    fillIndices(const void* data, int count=0, int offset=0)=0;

    int				getVertexCount() const				{ return _vxCount; }
    VertexType		getVertexType() const				{ return _vxType; }
    BufferUsage		getUsage() const					{ return _usage; }

    int				getIndexCount() const               { return _idxCount; }
    IndexType		getIndexType() const                { return _idxType; }

};

#endif // IGEOMETRYBUFFER_H
