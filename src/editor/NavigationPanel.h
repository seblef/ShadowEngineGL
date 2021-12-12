#pragma once

namespace Editor
{

class NavigationPanel
{
protected:

    bool _open;

    void drawResourceType(unsigned int type);

public:
    NavigationPanel();
    ~NavigationPanel();

    void draw();
    void setOpen(bool open) { _open = open; }
    bool isOpen() const { return _open; }
};

}