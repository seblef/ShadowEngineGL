#pragma once

namespace Editor
{

class IEditorElement
{
public:

    IEditorElement() {};
    virtual ~IEditorElement() {};

    virtual void draw()=0;
};

}