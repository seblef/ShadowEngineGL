#include "EditorSystem.h"
#include "EdMaterial.h"
#include "Helpers.h"
#include "MainMenu.h"
#include "MaterialWindow.h"
#include "Resources.h"
#include "imgui/imgui.h"
#include "filedialog/ImFileDialog.h"
#include "../loguru.hpp"

namespace Editor
{

void MainMenu::draw()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::BeginMenu("Open"))
            {
                if(ImGui::MenuItem("Map"))
                    ifd::FileDialog::Instance().Open("MapsFileDialog", "Open a map", "Map file (*.yaml){.yaml}", false);
                if(ImGui::MenuItem("Material"))
                    ifd::FileDialog::Instance().Open("MaterialsFileDialog", "Open a material", "Material file (*.yaml){.yaml}", false);
                if(ImGui::MenuItem("Particle system"))
                    ifd::FileDialog::Instance().Open("PSFileDialog", "Open a particle system", "Particle system file (*.yaml){.yaml}", false);
                if(ImGui::MenuItem("Actor"))
                    ifd::FileDialog::Instance().Open("ActorsFileDialog", "Open an actor", "Actor file (*.act){.act}", false);
                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Quit"))
                EditorSystem::getSingletonRef().quit();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View"))
        {
            NavigationPanel& panel = EditorSystem::getSingletonRef().getNavigation();
            bool open_nav = panel.isOpen();
            ImGui::MenuItem("Navigation panel", 0, &open_nav);
            panel.setOpen(open_nav);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    updateFileDialogs();
}

void MainMenu::updateFileDialogs()
{
    if(ifd::FileDialog::Instance().IsDone("MapsFileDialog"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string filename = ifd::FileDialog::Instance().GetResult();
            loadMap(filename);
        }
        ifd::FileDialog::Instance().Close();
    }

    if(ifd::FileDialog::Instance().IsDone("MaterialsFileDialog"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string filename = ifd::FileDialog::Instance().GetResult();
            loadMaterial(filename);
        }
        ifd::FileDialog::Instance().Close();
    }

    if(ifd::FileDialog::Instance().IsDone("PSFileDialog"))
        ifd::FileDialog::Instance().Close();

    if(ifd::FileDialog::Instance().IsDone("ActorsFileDialog"))
        ifd::FileDialog::Instance().Close();
}

bool MainMenu::loadMap(const std::string& mapFilename)
{
    LOG_S(INFO) << "Loading map " << mapFilename;
    return true;
}

bool MainMenu::loadMaterial(const std::string& matFileName)
{
    LOG_S(INFO) << "Load material " << matFileName;
    std::string matName = removeExtension(getRelativePath(matFileName));

    if(Resources::getSingletonRef().exists(RES_MATERIAL, matName))
        return true;

    EdMaterial *material = new EdMaterial(matName);
    if(material->isValid())
    {
        Resources::getSingletonRef().add(RES_MATERIAL, material, matName);
        new MaterialWindow(material);
        return true;
    }
    else
    {
        delete material;
        return false;
    }
}

}