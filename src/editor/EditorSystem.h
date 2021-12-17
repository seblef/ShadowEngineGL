#pragma once

#include "EdCamera.h"
#include "MainMenu.h"
#include "NavigationPanel.h"
#include "../core/TSingleton.h"
#include <list>

class IMedia;
class Map;

namespace YAML
{
    class Node;
}

namespace Editor
{

class EdMap;
class IWindow;
class PreviewResources;

class EditorSystem : public Core::TSingleton<Editor::EditorSystem>
{
protected:

    bool _canQuit;
    IMedia* _media;
    MainMenu _mainMenu;
    NavigationPanel _navPanel;
    std::list<IWindow*> _openWindows;

    EdCamera _camera;

    PreviewResources *_previewRes;

    EdMap* _map;

    void initUI();
    void shutdownUI();

    void drawWindows();

public:

    EditorSystem(IMedia* media, const YAML::Node& cfg);
    ~EditorSystem();

    bool update();

    void onResize(int width, int height);
    void quit();

    NavigationPanel& getNavigation() { return _navPanel; }
    PreviewResources* getPreviewResources() const { return _previewRes; }

    void openWindow(IWindow* win);

    void loadMap(const std::string& filename);
};


}
