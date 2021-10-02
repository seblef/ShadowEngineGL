#pragma once

class IDepthTexture
{
protected:

    int             _width;
    int             _height;

public:

    IDepthTexture(int w, int h) : _width(w), _height(h)     {}
    virtual ~IDepthTexture()                                {}

    int             getWidth() const                        { return _width; }
    int             getHeight() const                       { return _height; }

    virtual void    set(int stage)=0;
};
