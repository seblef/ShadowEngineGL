#pragma once

#include "IEditorElement.h"
#include <string>

namespace Editor
{

class MainMenu : public IEditorElement
{
protected:
    void updateFileDialogs();

    bool loadMap(const std::string& mapFilename);

public:

    MainMenu() {}
    ~MainMenu() {}

    void draw();
};

}