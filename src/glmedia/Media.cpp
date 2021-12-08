
#include "Media.h"
#include "GLVideoDevice.h"
#include "ALSoundDevice.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../mediacommon/EventPump.h"
#include "../mediacommon/KeyboardKeys.h"
#include "../loguru.hpp"
#include <assert.h>
#include <memory.h>


static const int        g_Keys[]={
    GLFW_KEY_SPACE,
    GLFW_KEY_APOSTROPHE,
    GLFW_KEY_COMMA,
    GLFW_KEY_MINUS,
    GLFW_KEY_PERIOD,
    GLFW_KEY_SLASH,
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
     GLFW_KEY_3,
     GLFW_KEY_4,
     GLFW_KEY_5,
     GLFW_KEY_6,
     GLFW_KEY_7,
     GLFW_KEY_8,
     GLFW_KEY_9,
     GLFW_KEY_SEMICOLON,
     GLFW_KEY_EQUAL,
     GLFW_KEY_A,
     GLFW_KEY_B,
     GLFW_KEY_C,
     GLFW_KEY_D,
     GLFW_KEY_E,
     GLFW_KEY_F,
     GLFW_KEY_G,
     GLFW_KEY_H,
     GLFW_KEY_I,
     GLFW_KEY_J,
     GLFW_KEY_K,
     GLFW_KEY_L,
     GLFW_KEY_M,
     GLFW_KEY_N,
     GLFW_KEY_O,
     GLFW_KEY_P,
     GLFW_KEY_Q,
     GLFW_KEY_R,
     GLFW_KEY_S,
     GLFW_KEY_T,
     GLFW_KEY_U,
     GLFW_KEY_V,
     GLFW_KEY_W,
     GLFW_KEY_X,
     GLFW_KEY_Y,
     GLFW_KEY_Z,
     GLFW_KEY_LEFT_BRACKET,
     GLFW_KEY_BACKSLASH,
     GLFW_KEY_RIGHT_BRACKET,
     GLFW_KEY_GRAVE_ACCENT,
     GLFW_KEY_WORLD_1,
     GLFW_KEY_WORLD_2,
     GLFW_KEY_ESCAPE,
     GLFW_KEY_ENTER,
     GLFW_KEY_TAB,
     GLFW_KEY_BACKSPACE,
     GLFW_KEY_INSERT,
     GLFW_KEY_DELETE,
     GLFW_KEY_RIGHT,
     GLFW_KEY_LEFT,
     GLFW_KEY_DOWN,
     GLFW_KEY_UP,
     GLFW_KEY_PAGE_UP,
     GLFW_KEY_PAGE_DOWN,
     GLFW_KEY_HOME,
     GLFW_KEY_END,
     GLFW_KEY_CAPS_LOCK,
     GLFW_KEY_SCROLL_LOCK,
     GLFW_KEY_NUM_LOCK,
     GLFW_KEY_PRINT_SCREEN,
     GLFW_KEY_PAUSE,
     GLFW_KEY_F1,
     GLFW_KEY_F2,
     GLFW_KEY_F3,
     GLFW_KEY_F4,
     GLFW_KEY_F5,
     GLFW_KEY_F6,
     GLFW_KEY_F7,
     GLFW_KEY_F8,
     GLFW_KEY_F9,
     GLFW_KEY_F10,
     GLFW_KEY_F11,
     GLFW_KEY_F12,
     GLFW_KEY_F13,
     GLFW_KEY_F14,
     GLFW_KEY_F15,
     GLFW_KEY_F16,
     GLFW_KEY_F17,
     GLFW_KEY_F18,
     GLFW_KEY_F19,
     GLFW_KEY_F20,
     GLFW_KEY_F21,
     GLFW_KEY_F22,
     GLFW_KEY_F23,
     GLFW_KEY_F24,
     GLFW_KEY_F25,
     GLFW_KEY_KP_0,
     GLFW_KEY_KP_1,
     GLFW_KEY_KP_2,
     GLFW_KEY_KP_3,
     GLFW_KEY_KP_4,
     GLFW_KEY_KP_5,
     GLFW_KEY_KP_6,
     GLFW_KEY_KP_7,
     GLFW_KEY_KP_8,
     GLFW_KEY_KP_9,
     GLFW_KEY_KP_DECIMAL,
     GLFW_KEY_KP_DIVIDE,
     GLFW_KEY_KP_MULTIPLY,
     GLFW_KEY_KP_SUBTRACT,
     GLFW_KEY_KP_ADD,
     GLFW_KEY_KP_ENTER,
     GLFW_KEY_KP_EQUAL,
     GLFW_KEY_LEFT_SHIFT,
     GLFW_KEY_LEFT_CONTROL,
     GLFW_KEY_LEFT_ALT,
     GLFW_KEY_LEFT_SUPER,
     GLFW_KEY_RIGHT_SHIFT,
     GLFW_KEY_RIGHT_CONTROL,
     GLFW_KEY_RIGHT_ALT,
     GLFW_KEY_RIGHT_SUPER,
     GLFW_KEY_MENU,
     GLFW_KEY_LAST
};

IMedia *createGLMedia(int width, int height, bool windowed, bool multithread, unsigned int input_flags)
{
    return new Media(width,height,windowed,input_flags);
}


float mouseZOffset = 0.f;


void scrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{
    mouseZOffset = (float)yoffset;
}


Media::Media(int width, int height, bool windowed, unsigned int input_flags) : _mouseLastZ(0.f)
{
    initKeyMap();

    if(!glfwInit())
    {
        assert(0 && "x [Media::Media]: glfwInit failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
    GLFWmonitor *m = *monitors;
    for(int i=0; i<monitorCount; ++i)
        LOG_S(INFO) << "Found monitor " << glfwGetMonitorName(m);

    GLFWmonitor *mon = windowed ? 0 : monitors[0];

    _win=glfwCreateWindow(width,height,"Shadow",mon,0);
    assert(_win!=0);

    glfwMakeContextCurrent(_win);

    glewExperimental=true; // Needed in core profile
    assert(glewInit() == GLEW_OK);

    if(!windowed)
        glfwSetInputMode(_win,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    glfwSetInputMode(_win, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(_win,GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    double x,y;
    glfwGetCursorPos(_win,&x,&y);
    _mouseLastX=(int)x;
    _mouseLastY=(int)y;

    glfwSetScrollCallback(_win, scrollCallback);

    _video=new GLVideoDevice(width,height,windowed,_win);
    _audio=new ALSoundDevice;
}

Media::~Media()
{
    glfwDestroyWindow(_win);
    glfwTerminate();
}

void Media::update()
{
    glfwPollEvents();

    //      Keyboard

    unsigned int s=_keyMap.size();
    for(unsigned int i=0;i<s;++i)
    {
        if(glfwGetKey(_win,g_Keys[i])==GLFW_PRESS)
        {
           if(!_lastKeys[i])
            {
                _lastKeys[i]=true;
                EventPump::getSingletonRef().postEvent(Event(ET_KEYDOWN,_keyMap[g_Keys[i]]));
            }
        }
        else if(_lastKeys[i])
        {
            _lastKeys[i]=false;
            EventPump::getSingletonRef().postEvent(Event(ET_KEYUP,_keyMap[g_Keys[i]]));
        }
    }

    for(unsigned int i=0;i<3;++i)
    {
        if(glfwGetMouseButton(_win,i)==GLFW_PRESS)
        {
            if(!_lastMouseButtons[i])
            {
                _lastMouseButtons[i]=true;
                EventPump::getSingletonRef().postEvent(Event(ET_MOUSEBUTTONDOWN,i));
            }
         }
        else if(_lastMouseButtons[i])
        {
            _lastMouseButtons[i]=false;
            EventPump::getSingletonRef().postEvent(Event(ET_MOUSEBUTTONUP,i));
        }
    }

    double x,y;
    int mx,my;
    glfwGetCursorPos(_win,&x,&y);
    mx=(int)x;
    my=(int)y;

    if(mx!=_mouseLastX || my!=_mouseLastY)
    {
        EventPump::getSingletonRef().postEvent(Event(ET_MOUSEMOVE,mx - _mouseLastX,my - _mouseLastY));

        _mouseLastX=mx;
        _mouseLastY=my;
    }

    if(mouseZOffset != 0.f)
    {
        EventPump::getSingletonRef().postEvent(Event(ET_MOUSEWHEEL, (int)mouseZOffset));
        mouseZOffset = 0.f;
    }

    if(glfwWindowShouldClose(_win))
        EventPump::getSingletonRef().postEvent(Event(ET_CLOSE));
}

void Media::initKeyMap()
{
    _keyMap.insert(make_pair((int)GLFW_KEY_SPACE,(int)DIK_SPACE));
    _keyMap.insert(make_pair(GLFW_KEY_APOSTROPHE, DIK_APOSTROPHE));
    _keyMap.insert(make_pair(GLFW_KEY_COMMA, DIK_COMMA));
    _keyMap.insert(make_pair(GLFW_KEY_MINUS, DIK_MINUS));
    _keyMap.insert(make_pair(GLFW_KEY_PERIOD,DIK_PERIOD));
    _keyMap.insert(make_pair(GLFW_KEY_SLASH, DIK_SLASH));
    _keyMap.insert(make_pair(GLFW_KEY_0, DIK_0));
    _keyMap.insert(make_pair(GLFW_KEY_1, DIK_1));
    _keyMap.insert(make_pair(GLFW_KEY_2, DIK_2));
    _keyMap.insert(make_pair(GLFW_KEY_3, DIK_3));
    _keyMap.insert(make_pair(GLFW_KEY_4, DIK_4));
    _keyMap.insert(make_pair(GLFW_KEY_5, DIK_5));
    _keyMap.insert(make_pair(GLFW_KEY_6, DIK_6));
    _keyMap.insert(make_pair(GLFW_KEY_7, DIK_7));
    _keyMap.insert(make_pair(GLFW_KEY_8, DIK_8));
    _keyMap.insert(make_pair(GLFW_KEY_9, DIK_9));
    _keyMap.insert(make_pair(GLFW_KEY_SEMICOLON, DIK_SEMICOLON));
    _keyMap.insert(make_pair(GLFW_KEY_EQUAL, DIK_EQUALS));
    _keyMap.insert(make_pair(GLFW_KEY_A,DIK_A));
    _keyMap.insert(make_pair(GLFW_KEY_B,DIK_B));
    _keyMap.insert(make_pair(GLFW_KEY_C, DIK_C));
    _keyMap.insert(make_pair(GLFW_KEY_D,DIK_D));
    _keyMap.insert(make_pair(GLFW_KEY_E,DIK_E));
    _keyMap.insert(make_pair(GLFW_KEY_F,DIK_F));
    _keyMap.insert(make_pair(GLFW_KEY_G,DIK_G));
    _keyMap.insert(make_pair(GLFW_KEY_H,DIK_H));
    _keyMap.insert(make_pair(GLFW_KEY_I,DIK_I));
    _keyMap.insert(make_pair(GLFW_KEY_J,DIK_J));
    _keyMap.insert(make_pair(GLFW_KEY_K,DIK_K));
    _keyMap.insert(make_pair(GLFW_KEY_L,DIK_L));
    _keyMap.insert(make_pair(GLFW_KEY_M,DIK_M));
    _keyMap.insert(make_pair(GLFW_KEY_N,DIK_N));
    _keyMap.insert(make_pair(GLFW_KEY_O,DIK_O));
    _keyMap.insert(make_pair(GLFW_KEY_P,DIK_P));
    _keyMap.insert(make_pair(GLFW_KEY_Q,DIK_Q));
    _keyMap.insert(make_pair(GLFW_KEY_R,DIK_R));
    _keyMap.insert(make_pair(GLFW_KEY_S,DIK_S));
    _keyMap.insert(make_pair(GLFW_KEY_T,DIK_T));
    _keyMap.insert(make_pair(GLFW_KEY_U,DIK_U));
    _keyMap.insert(make_pair(GLFW_KEY_V,DIK_V));
    _keyMap.insert(make_pair(GLFW_KEY_W,DIK_W));
    _keyMap.insert(make_pair(GLFW_KEY_X,DIK_X));
    _keyMap.insert(make_pair(GLFW_KEY_Y,DIK_Y));
    _keyMap.insert(make_pair(GLFW_KEY_Z,DIK_Z));
    _keyMap.insert(make_pair(GLFW_KEY_LEFT_BRACKET,DIK_LBRACKET));
    _keyMap.insert(make_pair(GLFW_KEY_BACKSLASH, DIK_BACKSLASH));
    _keyMap.insert(make_pair(GLFW_KEY_RIGHT_BRACKET, DIK_RBRACKET));
    _keyMap.insert(make_pair(GLFW_KEY_WORLD_1,DIK_SPACE));
    _keyMap.insert(make_pair(GLFW_KEY_WORLD_2,DIK_SPACE));
    _keyMap.insert(make_pair(GLFW_KEY_ESCAPE,DIK_ESCAPE));
    _keyMap.insert(make_pair(GLFW_KEY_ENTER, DIK_RETURN));
    _keyMap.insert(make_pair(GLFW_KEY_TAB,DIK_TAB));
    _keyMap.insert(make_pair(GLFW_KEY_BACKSPACE, DIK_BACK));
    _keyMap.insert(make_pair(GLFW_KEY_INSERT, DIK_INSERT));
    _keyMap.insert(make_pair(GLFW_KEY_DELETE, DIK_DELETE));
    _keyMap.insert(make_pair(GLFW_KEY_RIGHT, DIK_RIGHT));
    _keyMap.insert(make_pair(GLFW_KEY_LEFT, DIK_LEFT));
    _keyMap.insert(make_pair(GLFW_KEY_UP, DIK_UP));
    _keyMap.insert(make_pair(GLFW_KEY_DOWN, DIK_DOWN));
    _keyMap.insert(make_pair(GLFW_KEY_PAGE_UP, DIK_PRIOR));
    _keyMap.insert(make_pair(GLFW_KEY_PAGE_DOWN, DIK_NEXT));
    _keyMap.insert(make_pair(GLFW_KEY_HOME, DIK_HOME));
    _keyMap.insert(make_pair(GLFW_KEY_END, DIK_END));
    _keyMap.insert(make_pair(GLFW_KEY_CAPS_LOCK, DIK_CAPITAL));
    _keyMap.insert(make_pair(GLFW_KEY_SCROLL_LOCK, DIK_SCROLL));
    _keyMap.insert(make_pair(GLFW_KEY_NUM_LOCK, DIK_NUMLOCK));
    _keyMap.insert(make_pair(GLFW_KEY_PRINT_SCREEN, DIK_SPACE));
    _keyMap.insert(make_pair(GLFW_KEY_PAUSE, DIK_PAUSE));
    _keyMap.insert(make_pair(GLFW_KEY_F1, DIK_F1));
    _keyMap.insert(make_pair(GLFW_KEY_F2, DIK_F2));
    _keyMap.insert(make_pair(GLFW_KEY_F3, DIK_F3));
    _keyMap.insert(make_pair(GLFW_KEY_F4, DIK_F4));
    _keyMap.insert(make_pair(GLFW_KEY_F5, DIK_F5));
    _keyMap.insert(make_pair(GLFW_KEY_F6, DIK_F6));
    _keyMap.insert(make_pair(GLFW_KEY_F7, DIK_F7));
    _keyMap.insert(make_pair(GLFW_KEY_F8, DIK_F8));
    _keyMap.insert(make_pair(GLFW_KEY_F9, DIK_F9));
    _keyMap.insert(make_pair(GLFW_KEY_F10, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F11, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F12, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F13, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F14, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F15, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F16, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F17, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F18, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F19, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F20, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F21, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F22, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F23, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F24, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_F25, DIK_F10));
    _keyMap.insert(make_pair(GLFW_KEY_KP_0, DIK_NUMPAD0));
    _keyMap.insert(make_pair(GLFW_KEY_KP_1, DIK_NUMPAD1));
    _keyMap.insert(make_pair(GLFW_KEY_KP_2, DIK_NUMPAD2));
    _keyMap.insert(make_pair(GLFW_KEY_KP_3, DIK_NUMPAD3));
    _keyMap.insert(make_pair(GLFW_KEY_KP_4, DIK_NUMPAD4));
    _keyMap.insert(make_pair(GLFW_KEY_KP_5, DIK_NUMPAD5));
    _keyMap.insert(make_pair(GLFW_KEY_KP_6, DIK_NUMPAD6));
    _keyMap.insert(make_pair(GLFW_KEY_KP_7, DIK_NUMPAD7));
    _keyMap.insert(make_pair(GLFW_KEY_KP_8, DIK_NUMPAD8));
    _keyMap.insert(make_pair(GLFW_KEY_KP_9, DIK_NUMPAD9));
    _keyMap.insert(make_pair(GLFW_KEY_KP_DECIMAL,DIK_DECIMAL));
    _keyMap.insert(make_pair(GLFW_KEY_KP_DIVIDE, DIK_DIVIDE));
    _keyMap.insert(make_pair(GLFW_KEY_KP_MULTIPLY, DIK_MULTIPLY));
    _keyMap.insert(make_pair(GLFW_KEY_KP_SUBTRACT, DIK_SUBTRACT));
    _keyMap.insert(make_pair(GLFW_KEY_KP_ADD, DIK_ADD));
    _keyMap.insert(make_pair(GLFW_KEY_KP_ENTER, DIK_NUMPADENTER));
    _keyMap.insert(make_pair(GLFW_KEY_KP_EQUAL, DIK_NUMPADEQUALS));
    _keyMap.insert(make_pair(GLFW_KEY_LEFT_SHIFT, DIK_LSHIFT));
    _keyMap.insert(make_pair(GLFW_KEY_LEFT_CONTROL, DIK_LCONTROL));
    _keyMap.insert(make_pair(GLFW_KEY_LEFT_ALT, DIK_LALT));
    _keyMap.insert(make_pair(GLFW_KEY_LEFT_SUPER, DIK_LWIN));
    _keyMap.insert(make_pair(GLFW_KEY_RIGHT_SHIFT, DIK_RSHIFT));
    _keyMap.insert(make_pair(GLFW_KEY_RIGHT_CONTROL, DIK_RCONTROL));
    _keyMap.insert(make_pair(GLFW_KEY_RIGHT_ALT, DIK_RALT));
    _keyMap.insert(make_pair(GLFW_KEY_RIGHT_SUPER, DIK_RWIN));
    _keyMap.insert(make_pair(GLFW_KEY_MENU, DIK_APPS));
    _keyMap.insert(make_pair(GLFW_KEY_LAST, DIK_SPACE));

    _lastKeys=new bool[_keyMap.size()];
    memset(_lastKeys,0,sizeof(bool) * _keyMap.size());

    for(int i=0;i<3;++i)
        _lastMouseButtons[i]=false;
}

void* Media::getWindow()
{
    return (void*)_win;
}