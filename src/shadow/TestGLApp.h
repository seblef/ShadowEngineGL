#ifndef TESTGLAPP_H
#define TESTGLAPP_H

#include "../MediaCommon.h"

class TestGLApp
{
protected:

    void            createPlane(IVideoDevice *d);

public:

    TestGLApp()     {}

    void            run();
};

#endif // TESTGLAPP_H
