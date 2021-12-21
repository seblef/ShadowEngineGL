#include "EditorSystem.h"
#include "CameraTool.h"
#include "Drawer.h"
#include "EdCamera.h"
#include "EdMap.h"
#include "IWindow.h"
#include "PreviewResources.h"
#include "Resources.h"
#include "Selection.h"
#include "SelectionTool.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "filedialog/ImFileDialog.h"
#include "../mediacommon/IMedia.h"
#include "../mediacommon/IVideoDevice.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>


namespace Editor
{

EditorSystem::EditorSystem(IMedia* media, const YAML::Node& cfg) :
    _canQuit(false),
    _media(media),
    _map(0),
    _currentTool(0),
    _tools(media->getVideo())
{
    _previewRes = new PreviewResources(media->getVideo());
    new Resources;
    new Drawer(media->getVideo(), _previewRes);
    new Selection;
    _camera.onResize(media->getVideo()->getResWidth(), media->getVideo()->getResHeight());
    initUI();
    setTool(TOOL_SELECTION);
}

EditorSystem::~EditorSystem()
{
    if(_currentTool)
        delete _currentTool;
    if(_map)
        delete _map;
    shutdownUI();
    Resources::deleteSingleton();
    delete _previewRes;
    Drawer::deleteSingleton();
    Selection::deleteSingleton();
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    EditorSystem::getSingletonRef().onResize(width, height);
}


void EditorSystem::initUI()
{
    GLFWwindow *window = (GLFWwindow*)_media->getWindow();
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

	ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
		GLuint tex;

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return (void*)tex;
	};
	ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
		GLuint texID = (GLuint)((uintptr_t)tex);
		glDeleteTextures(1, &texID);
	};
}

void EditorSystem::shutdownUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


bool EditorSystem::update()
{
    GLFWwindow *window = (GLFWwindow*)_media->getWindow();

	_media->getVideo()->newFrame();

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    processInput();
    Selection::getSingletonRef().refresh();

    Drawer::getSingletonRef().draw(_camera);

    _mainMenu.draw();
    _navPanel.draw();
    _tools.draw();
    drawWindows();

    bool open;
    ImGui::ShowDemoWindow(&open);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return glfwWindowShouldClose(window) || _canQuit;
}

void EditorSystem::onResize(int width, int height)
{
    Drawer::getSingletonRef().onResize(width, height);
    _camera.onResize(width, height);
}

void EditorSystem::quit()
{
    _canQuit = true;
}

void EditorSystem::openWindow(IWindow* win)
{
    _openWindows.push_back(win);
}

void EditorSystem::drawWindows()
{
    std::vector<IWindow*> closedWindows;
    for(auto& win : _openWindows)
    {
        win->draw();
        if(!win->isOpen())
            closedWindows.push_back(win);
    }

    for(auto const& win : closedWindows)
    {
        delete win;
        _openWindows.remove(win);
    }
}

void EditorSystem::loadMap(const std::string& filename)
{
    Drawer::getSingletonRef().onNewMap();
    if(_map)
        delete _map;
    LOG_S(INFO) << "Loading map " << filename;
    _map = new EdMap(filename);
    _camera.initialize(_map->getWidth(), _map->getHeight());
}

void EditorSystem::setTool(ToolType type)
{
    if(_currentTool)
    {
        delete _currentTool;
        _currentTool = 0;
    }

    ImGuiIO& io = ImGui::GetIO();
    int mouseX = (int)io.MousePos.x;
    int mouseY = (int)io.MousePos.y;
    int mouseWheel = (int)io.MouseWheel;

    switch(type)
    {
    case TOOL_NULL:
        _currentTool = new ITool(TOOL_NULL, mouseX, mouseY, mouseWheel);
        break;
    case TOOL_CAMERA:
        _currentTool = new CameraTool(&_camera, mouseX, mouseY, mouseWheel);
        break;
    case TOOL_SELECTION:
        _currentTool = new SelectionTool(mouseX, mouseY, mouseWheel);
        break;
    }

    _tools.setCurrentTool(type);
}

void EditorSystem::processInput()
{
    if(!_currentTool)
        return;

    ImGuiIO& io = ImGui::GetIO();
    if(!io.WantCaptureMouse)
    {
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            _currentTool->onMouseButtonPressed(MB_LEFT);
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            _currentTool->onMouseButtonPressed(MB_RIGHT);
    }

    if(ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        _currentTool->onMouseButtonReleased(MB_LEFT);
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        _currentTool->onMouseButtonReleased(MB_RIGHT);
    
    int deltaX = (int)io.MouseDelta.x;
    int deltaY = (int)io.MouseDelta.y;
    if(deltaX != 0 || deltaY !=0)
        _currentTool->onMouseMove(deltaX, deltaY);
    
    int wheel = (int)io.MouseWheel;
    _currentTool->onMouseWheel(wheel);

    _currentTool->setCtrlDown(io.KeyCtrl);
}

}
