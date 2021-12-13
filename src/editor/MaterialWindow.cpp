#include "MaterialWindow.h"
#include "EditorSystem.h"
#include "EdMaterial.h"
#include "Helpers.h"
#include "PreviewMesh.h"
#include "Resources.h"
#include "imgui/imgui.h"
#include "filedialog/ImFileDialog.h"
#include "../renderer/Material.h"
#include "../renderer/MaterialAnimation.h"
#include "../renderer/MaterialFadeAnimation.h"
#include "../renderer/MaterialTextureAnimation.h"
#include "../renderer/MaterialUVAnimation.h"
#include "../renderer/Renderer.h"


namespace Editor
{

MaterialWindow::MaterialWindow(EdMaterial* material) :
    _material(material),
    _save(new EdMaterialSave(material->getMaterial())),
    _preview(0),
    _startTime(std::chrono::steady_clock::now()),
    _changed(false)
{
    _preview = new PreviewMesh(
        0,
        material->getMaterial(),
        Renderer::getSingletonRef().getVideoDevice(),
        EditorSystem::getSingletonRef().getPreviewResources(),
        128, 128
    );
}

MaterialWindow::~MaterialWindow()
{
    delete _save;
    if(_preview)
        delete _preview;
}

void MaterialWindow::draw()
{
    std::string winName = "Material " + _material->getName();
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;
    if(_changed)
        flags |= ImGuiWindowFlags_UnsavedDocument;
    if(ImGui::Begin(winName.c_str(), &_open, flags))
    {
        char buffer[64];
        strcpy(buffer, _material->getName().c_str());
        ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));

        std::chrono::duration<float> time = std::chrono::steady_clock::now() - _startTime;
        
        if(_preview)
            _preview->draw(time.count());

        drawColors();
        drawTextures();
        drawUV();
        drawBlendAndCull();
        drawAnimations();
        drawGame();
        drawButtons();
    }

    ImGui::End();
}

void MaterialWindow::drawColors()
{
    if(ImGui::CollapsingHeader("Colors"))
    {
        Color ambient(_material->getMaterial()->getStdBuffer().getAmbient());
        Color diffuse(_material->getMaterial()->getStdBuffer().getDiffuse());
        Color specular(_material->getMaterial()->getStdBuffer().getSpecular());
        float shininess = _material->getMaterial()->getStdBuffer().getShininess();
        float specIntensity = _material->getMaterial()->getStdBuffer().getSpecIntensity();
        float reflectivity = _material->getMaterial()->getStdBuffer().getReflection();

        ImGui::ColorEdit4("Ambient", (float*)&ambient);
        ImGui::ColorEdit4("Diffuse", (float*)&diffuse);
        ImGui::ColorEdit4("Specular", (float*)&specular);
        ImGui::Separator();
        ImGui::InputFloat("Shininess", &shininess, .1f);
        ImGui::InputFloat("Specular intensity", &specIntensity, 1.f);
        ImGui::InputFloat("Reflectivity", &reflectivity, .1f);

        if(ambient!=_material->getMaterial()->getStdBuffer().getAmbient())
        {
            _material->getMaterial()->getStdBuffer().setAmbient(ambient);
            _changed = true;
        }
        if(diffuse!=_material->getMaterial()->getStdBuffer().getDiffuse())
        {
            _material->getMaterial()->getStdBuffer().setDiffuse(diffuse);
            _changed = true;
        }
        if(specular!=_material->getMaterial()->getStdBuffer().getSpecular())
        {
            _material->getMaterial()->getStdBuffer().setSpecular(specular);
            _changed = true;
        }
        if(shininess!=_material->getMaterial()->getStdBuffer().getShininess())
        {
            _material->getMaterial()->getStdBuffer().setShininess(shininess);
            _changed = true;
        }
        if(specIntensity!=_material->getMaterial()->getStdBuffer().getSpecIntensity())
        {
            _material->getMaterial()->getStdBuffer().setSpecIntensity(specIntensity);
            _changed = true;
        }
        if(reflectivity!=_material->getMaterial()->getStdBuffer().getReflection())
        {
            _material->getMaterial()->getStdBuffer().setReflection(reflectivity);
            _changed = true;
        }
    }
}

void MaterialWindow::drawTextures()
{
    const char * const slotNames[] = {
        "Diffuse",
        "Specular",
        "Normal",
        "Emissive",
        "Environment"
    };
    if(ImGui::CollapsingHeader("Textures"))
    {
        for(int i=0;i<TS_COUNT;++i)
        {
            std::string texName(_material->getMaterial()->getTexture((TextureSlot)i)->getName());
            std::string fileDialogKey("Texture_");
            fileDialogKey += slotNames[i];

            ImGui::Text(slotNames[i]);
            ImGui::SameLine();
            if(ImGui::Button(texName.c_str()))
            {
                ifd::FileDialog::Instance().Open(
                    fileDialogKey,
                    "Select a texture",
                    "Image file (*.*)",
                    false
                );
            }

            if(ifd::FileDialog::Instance().IsDone(fileDialogKey))
            {
                if(ifd::FileDialog::Instance().HasResult())
                {
                    std::string newTex(ifd::FileDialog::Instance().GetResult());
                    newTex = getRelativePath(newTex);

                    if(newTex != texName)
                    {
                        _material->getMaterial()->setTexture((TextureSlot)i, newTex);
                        _changed = true;
                    }
                }
                ifd::FileDialog::Instance().Close();
            }
        }
    }
}

void MaterialWindow::drawUV()
{
    if(ImGui::CollapsingHeader("UV Transform"))
    {
        Vector2 offset(_material->getMaterial()->getStdBuffer().getOffset());
        Vector2 scale(_material->getMaterial()->getStdBuffer().getScale());

        ImGui::SliderFloat2("Offset", (float*)&offset, 0.f, 1.f);
        ImGui::InputFloat2("Scale", (float*)&scale);

        if(offset!=_material->getMaterial()->getStdBuffer().getOffset())
        {
            _material->getMaterial()->getStdBuffer().setOffset(offset);
            _changed = true;
        }
        if(scale!=_material->getMaterial()->getStdBuffer().getScale())
        {
            _material->getMaterial()->getStdBuffer().setScale(scale);
            _changed = true;
        }
    }
}

void MaterialWindow::drawBlendAndCull()
{
    if(ImGui::CollapsingHeader("Blend and cull"))
    {
        IVideoDevice* device = Renderer::getSingletonRef().getVideoDevice();

        BlendState blendState = _material->getMaterial()->getBlendState();
        bool blendEnabled, newBlendEnabled;
        BlendMode srcBlend, destBlend;
        BlendMode newSrcBlend, newDestBlend;
        device->getBlendStateDesc(blendState, blendEnabled, srcBlend, destBlend);
        newSrcBlend = srcBlend;
        newDestBlend = destBlend;
        newBlendEnabled = blendEnabled;

        const char* const blendNames[] = {
            "Zero",
            "One",
            "Src color",
            "Inv src color",
            "Src alpha",
            "Inv src alpha",
            "Dest alpha",
            "Inv dest alpha",
            "Dest color",
            "Inv dest color",
            "Src alpha sat",
            "Both src alpha",
            "Both inv src alpha"
        };
        ImGui::Checkbox("Blend enabled", &newBlendEnabled);
        if(!blendEnabled)
            ImGui::BeginDisabled();
        
        ImGui::Combo("Source", (int*)&newSrcBlend, blendNames, IM_ARRAYSIZE(blendNames));
        ImGui::Combo("Destination", (int*)&newDestBlend, blendNames, IM_ARRAYSIZE(blendNames));

        if(!blendEnabled)
            ImGui::EndDisabled();

        if(newBlendEnabled != blendEnabled ||
           newSrcBlend != srcBlend ||
           newDestBlend != destBlend)
        {
            device->destroyBlendState(blendState);
            blendState = device->createBlendState(
                newBlendEnabled,
                newSrcBlend,
                newDestBlend
            );
            _material->getMaterial()->setBlendState(blendState);
            _changed = true;
        }

        ImGui::Separator();

        RenderState renderState = _material->getMaterial()->getRenderState();
        FillMode fill;
        CullMode cull, newCull;
        device->getRenderStateDesc(renderState, cull, fill);
        newCull = cull;

        const char* const cullNames[] = {
            "None",
            "Front",
            "Back"
        };
        ImGui::Combo("Cull mode", (int*)&newCull, cullNames, IM_ARRAYSIZE(cullNames));
        if(newCull != cull)
        {
            device->destroyRenderState(renderState);
            renderState = device->createRenderState(newCull, fill);
            _material->getMaterial()->setRenderState(renderState);
            _changed = true;
        }

        ImGui::Separator();

        bool newAlpha, alpha = (bool)_material->getMaterial()->getFlag(MF_ALPHA);
        bool newShadows, castNoShadows = (bool)_material->getMaterial()->getFlag(MF_CASTNOSHADOWS);
        newAlpha = alpha;
        newShadows = castNoShadows;

        ImGui::Checkbox("Transparent", &newAlpha);
        ImGui::Checkbox("Cast no shadows", &newShadows);

        if(newAlpha != alpha)
        {
            if(newAlpha)
                _material->getMaterial()->setFlag(MF_ALPHA);
            else
                _material->getMaterial()->unsetFlag(MF_ALPHA);
            _changed = true;
        }
        if(newShadows != castNoShadows)
        {
            if(newShadows)
                _material->getMaterial()->setFlag(MF_CASTNOSHADOWS);
            else
                _material->getMaterial()->unsetFlag(MF_CASTNOSHADOWS);
            _changed = true;
        }
    }
}

void MaterialWindow::drawAnimations()
{
    if(ImGui::CollapsingHeader("Animations"))
    {
        bool uvAnim = false, newuvAnim = false;
        bool texAnim = false, newTexAnim = false;
        bool fadeAnim = false, newFadeAnim = false;
        Vector2 offsetPerSec(Vector2::NullVector), newOffsetPerSec(Vector2::NullVector);
        float rotPerSec = 0.f, newRotPerSec = .0f;
        float animTime = 5.f, newAnimTime = 5.f;
        int tileCount = 1, newTileCount = 1;
        int tilePerRow = 1, newTilePerRow = 1;
        int tilePerCol = 1, newTilePerCol = 1;
        float startTime = .0f, newStartTime = .0f;
        float endTime = 1.f, newEndTime = 1.f;
        int texAnimIdx = -1, uvAnimIdx = -1, fadeAnimIdx = -1;

        for(int i=0; i<_material->getMaterial()->getAnimationCount(); ++i)
        {
            MaterialAnimation *anim = _material->getMaterial()->getAnimation(i);
            if(strcmp(anim->getAnimationName(), "fade") == 0)
            {
                fadeAnim = newFadeAnim = true;
                fadeAnimIdx = i;

                MaterialFadeAnimation* fade = (MaterialFadeAnimation*)anim;
                startTime = newStartTime = fade->getStartTime();
                endTime = newEndTime = fade->getEndTime();
            }
            else if(strcmp(anim->getAnimationName(), "texture") == 0)
            {
                texAnim = newTexAnim = true;
                texAnimIdx = i;

                MaterialTextureAnimation* tex = (MaterialTextureAnimation*)anim;
                animTime = newAnimTime = tex->getAnimTime();
                tileCount = newTileCount = tex->getTileCount();
                tilePerRow = newTilePerRow = tex->getTilePerRow();
                tilePerCol = newTilePerCol = tex->getTilePerColumn();
            }
            else if(strcmp(anim->getAnimationName(), "uv") == 0)
            {
                uvAnim = newuvAnim = true;
                uvAnimIdx = i;

                MaterialUVAnimation* uv = (MaterialUVAnimation*)anim;
                offsetPerSec = newOffsetPerSec = uv->getOffset();
                rotPerSec = newRotPerSec = uv->getRotation();
            }
        }

        ImGui::Checkbox("UV", &newuvAnim);
        if(!newuvAnim)
            ImGui::BeginDisabled();
        
        ImGui::InputFloat2("Offset/s", (float*)&newOffsetPerSec);
        ImGui::SliderFloat("Rotation/s", &newRotPerSec, .0f, 360.f);
        
        if(!newuvAnim)
            ImGui::EndDisabled();

        if(newuvAnim != uvAnim)
        {
            if(newuvAnim)
            {
                MaterialAnimation* anim = new MaterialUVAnimation(
                    _material->getMaterial(),
                    newOffsetPerSec,
                    newRotPerSec
                );
                _material->getMaterial()->addAnimation(anim);
            }
            else
                _material->getMaterial()->deleteAnimation(uvAnimIdx);
            _changed = true;
        }
        if(newuvAnim && (
           newOffsetPerSec != offsetPerSec ||
           newRotPerSec != rotPerSec))
        {
            MaterialUVAnimation *anim = (MaterialUVAnimation*)_material->getMaterial()->getAnimation(uvAnimIdx);
            anim->setOffset(newOffsetPerSec);
            anim->setRotation(newRotPerSec);
            _changed = true;
        }

        ImGui::Separator();

        ImGui::Checkbox("Texture", &newTexAnim);
        if(!newTexAnim)
            ImGui::BeginDisabled();

        ImGui::SliderFloat("Time", &newAnimTime, 0.1f, 60.f);
        ImGui::SliderInt("Tiles count", &newTileCount, 1, 128);
        ImGui::SliderInt("Tiles per row", &newTilePerRow, 1, 32);
        ImGui::SliderInt("Tiles per col", &newTilePerCol, 1, 32);

        if(!newTexAnim)
            ImGui::EndDisabled();

        if(texAnim != newTexAnim)
        {
            if(newTexAnim)
            {
                MaterialTextureAnimation* tex = new MaterialTextureAnimation(
                    _material->getMaterial(),
                    newAnimTime,
                    tilePerRow, tilePerCol,
                    tileCount
                );
                _material->getMaterial()->addAnimation(tex);
            }
            else
                _material->getMaterial()->deleteAnimation(texAnimIdx);
            _changed = true;
        }
        if(newTexAnim && (
            newAnimTime != animTime ||
            newTileCount != tileCount ||
            newTilePerRow != tilePerRow ||
            newTilePerCol != tilePerCol))
        {
            MaterialTextureAnimation* tex = (MaterialTextureAnimation*)_material->getMaterial()->getAnimation(texAnimIdx);
            tex->setAnimTime(newAnimTime);
            tex->setTileCount(newTileCount);
            tex->setTilePerRow(newTilePerRow);
            tex->setTilePerColumn(newTilePerCol);
            _changed = true;
        }

        ImGui::Separator();

        ImGui::Checkbox("Fade", &newFadeAnim);
        if(!newFadeAnim)
            ImGui::BeginDisabled();
        
        ImGui::InputFloat("Start time", &newStartTime, .1f, .5f);
        ImGui::InputFloat("End time", &newEndTime, .1f, .5f);
        
        if(!newFadeAnim)
            ImGui::EndDisabled();

        if(newFadeAnim != fadeAnim)
        {
            if(newFadeAnim)
            {
                MaterialFadeAnimation* fade = new MaterialFadeAnimation(
                    _material->getMaterial(),
                    newStartTime,
                    newEndTime
                );
                _material->getMaterial()->addAnimation(fade);
            }
            else
                _material->getMaterial()->deleteAnimation(fadeAnimIdx);
            _changed = true;
        }

        if(newFadeAnim && (
            newStartTime != startTime ||
            newEndTime != endTime
        ))
        {
            MaterialFadeAnimation* fade = (MaterialFadeAnimation*)_material->getMaterial()->getAnimation(fadeAnimIdx);
            fade->setStartTime(newStartTime);
            fade->setEndTime(newEndTime);
            _changed = true;
        }
    }
}

void MaterialWindow::drawGame()
{
    if(ImGui::CollapsingHeader("Game"))
    {
        std::string debris(_material->getDebrisMesh());
        ImGui::Text("Debris");
        ImGui::SameLine();
        if(ImGui::Button(debris.empty() ? "< >" : debris.c_str()))
        {
            ifd::FileDialog::Instance().Open(
                "debris_mesh",
                "Select a geometry",
                "Geometry file (*.geo; *.GEO){.geo,.GEO}",
                false
            );
        }

        if(ifd::FileDialog::Instance().IsDone("debris_mesh"))
        {
            if(ifd::FileDialog::Instance().HasResult())
            {
                std::string newDebris(ifd::FileDialog::Instance().GetResult());
                newDebris = getRelativePath(newDebris);

                if(debris != newDebris)
                {
                    _material->setDebrisMesh(newDebris);
                    _changed = true;
                }
            }
            ifd::FileDialog::Instance().Close();
        }    
    }
}

void MaterialWindow::drawButtons()
{
    ImGui::Separator();
    if(ImGui::Button("OK"))
        _open = false;
    ImGui::SameLine();
    if(ImGui::Button("Restore"))
    {
        _save->restore();
        _changed = false;
    }
    ImGui::SameLine();
    if(ImGui::Button("Export"))
    {
        ifd::FileDialog::Instance().Save(
            "material_export",
            "Material export",
            "Material file (*.yaml){.yaml}"
        );
    }

    if(ifd::FileDialog::Instance().IsDone("material_export"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string matFile(ifd::FileDialog::Instance().GetResult());
            _material->save(matFile);
            _changed = false;
        }
        ifd::FileDialog::Instance().Close();
    }    
}

}