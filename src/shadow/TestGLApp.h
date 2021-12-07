#pragma once

#include <string>

class IVideoDevice;


class TestGLApp
{
protected:

    void            createPlane(IVideoDevice *d);

public:

    TestGLApp()     {}

    void            run(const std::string& dataFolder);
};
