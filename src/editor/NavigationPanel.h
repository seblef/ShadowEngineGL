#pragma once

#include "Resources.h"

namespace Editor
{

class NavigationPanel
{
protected:

    bool _open;
    IResource* _selected[RES_COUNT];

    void drawResourceType(unsigned int type);
    void openResourceWindow(ResourceType type, IResource* res);

public:
    NavigationPanel();
    ~NavigationPanel();

    void draw();
    void setOpen(bool open) { _open = open; }
    bool isOpen() const { return _open; }
};

}