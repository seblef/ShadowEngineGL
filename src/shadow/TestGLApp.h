#pragma once

#include "../MediaCommon.h"

class TestGLApp
{
protected:

    void            createPlane(IVideoDevice *d);

public:

    TestGLApp()     {}

    void            run(const string& dataFolder);
};
