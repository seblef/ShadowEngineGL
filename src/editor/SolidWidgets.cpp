#include "SolidWidgets.h"
#include "EdMaterial.h"
#include "EdSolid.h"
#include "Helpers.h"
#include "PreviewMesh.h"
#include "Resources.h"
#include "../game/FilesStrings.h"
#include "../physic/PhysicDefs.h"
#include "imgui/imgui.h"
#include "filedialog/ImFileDialog.h"



namespace Editor
{


const int MaxMaterials = 256;


SolidWidgets::SolidWidgets(
    EdSolidTemplate* solid,
    PreviewMesh* preview
) :
    _solid(solid),
    _preview(preview)
{
}

bool SolidWidgets::draw()
{
    bool changed = false;

    ImGui::Text("Geometry");
    ImGui::SameLine();
    if(ImGui::Button(_solid->getGeometryName().c_str()))
    {
        ifd::FileDialog::Instance().Open(
            "solid_geo",
            "Select a geometry",
            "Geometry file (*.geo; *.GEO){.geo,.GEO}",
            false
        );
    }

    if(ifd::FileDialog::Instance().IsDone("solid_geo"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string newGeo(ifd::FileDialog::Instance().GetResult());
            newGeo = getRelativePath(newGeo);
            _solid->setGeometry(newGeo);
            _preview->setGeometryData(_solid->getMesh()->getGeometry());
            changed = true;
        }
        ifd::FileDialog::Instance().Close();
    }    

    int selMat = -1;
    const char* materials[MaxMaterials];
    int idx = 0;
    for(auto const& mat : Resources::getSingletonRef().getAll(RES_MATERIAL))
    {
        if(_solid->getMaterialName() == mat.first)
            selMat = idx;
        materials[idx] = mat.first.c_str();
        ++idx;

        if(idx >= MaxMaterials)
            break;
    }
    if(ImGui::Combo("Material", &selMat, materials, idx))
    {
        _solid->setMaterial(materials[selMat]);
        _preview->setMaterial(_solid->getMaterial()->getMaterial());
        changed = true;
    }

    int shape = (int)_solid->getShape();
    if(ImGui::Combo("Physic shape", &shape, g_PhysicShapeNames, IM_ARRAYSIZE(g_PhysicShapeNames)))
    {
        _solid->setShape((PhysicShape)shape);
        changed = true;
    }

    return changed;
}

}