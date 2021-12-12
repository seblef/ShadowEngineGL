#include "IWindow.h"
#include "EditorSystem.h"

namespace Editor
{

IWindow::IWindow() :
    _open(true)
{
    EditorSystem::getSingletonRef().openWindow(this);    
}

}