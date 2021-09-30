#ifndef IFRAMEBUFFER_H
#define IFRAMEBUFFER_H

#include "ITexture.h"

class IFrameBuffer
{
protected:

    int           _width;
    int           _height;
    bool          _depth;

public:

    IFrameBuffer(int width, int height, bool depth) : _width(width), _height(height), _depth(depth)     {}
    virtual ~IFrameBuffer()                                                                             {}

    virtual void    set()=0;
};

#endif // IFRAMEBUFFER_H
