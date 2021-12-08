#pragma once

#include "../core/TSingleton.h"
#include "MainMenu.h"

class IMedia;
class Map;

namespace YAML
{
    class Node;
}

namespace Editor
{

class EdCamera;

class EditorSystem : public Core::TSingleton<Editor::EditorSystem>
{
protected:

    bool _canQuit;
    IMedia* _media;
    MainMenu _mainMenu;

    EdCamera *_camera;

    void initUI();
    void shutdownUI();

public:

    EditorSystem(IMedia* media, const YAML::Node& cfg);
    ~EditorSystem();

    bool update();

    void quit();
};


}
