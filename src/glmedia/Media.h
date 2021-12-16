#pragma once

#include "../mediacommon/IMedia.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>

class Media : public IMedia
{
protected:

    GLFWwindow*         _win;
    bool*               _lastKeys;
    bool                _lastMouseButtons[3];
    int                 _mouseLastX;
    int                 _mouseLastY;
    float               _mouseLastZ;

    typedef std::map<int,int>    IntMap;
    IntMap              _keyMap;

    void                initKeyMap();

public:

    Media(int width, int height, bool windowed, unsigned int input_flags);
    ~Media();

    void                update();
    void*               getWindow();

};

IMedia*			createGLMedia(int width, int height, bool windowed, bool multithread, unsigned int input_flags);