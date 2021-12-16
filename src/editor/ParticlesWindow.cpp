#include "ParticlesWindow.h"
#include "EditorSystem.h"
#include "EdParticles.h"
#include "Helpers.h"
#include "PreviewParticles.h"
#include "filedialog/ImFileDialog.h"
#include "imgui/imgui.h"
#include "../mediacommon/ITexture.h"
#include "../particles/System.h"
#include "../particles/TextureSet.h"
#include "../renderer/Renderer.h"
#include <algorithm>


namespace Editor
{

const char* const RenderersNames[] = {
    "quad",
    "point"
};
const int RenderersCount = 2;
const char * const EmittersNames[] = {
    "point",
    "line",
    "explosion"
};
const int EmittersCount = 3;

const char* const BlendNames[] = {
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

int findIndex(const std::string& name, const char* const nameList[], int nameCount)
{
    for(int i=0;i<nameCount;++i)
        if(name == nameList[i])
            return i;
    return -1;
}


ParticlesWindow::ParticlesWindow(EdParticles* particles) :
    _particles(particles),
    _startTime(std::chrono::steady_clock::now()),
    _changed(false)
{
    particles->backup();
    particles->startEdit();

    _preview = std::unique_ptr<PreviewParticles>(
        new PreviewParticles(
            &particles->getTemplate(),
            Renderer::getSingletonRef().getVideoDevice(),
            EditorSystem::getSingletonRef().getPreviewResources(),
            256, 256
        )
    );
}

ParticlesWindow::~ParticlesWindow()
{
    _particles->stopEdit();
}

void ParticlesWindow::draw()
{
    std::string winName = "Particles " + _particles->getName();
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;
    if(_changed)
        flags |= ImGuiWindowFlags_UnsavedDocument;
    if(ImGui::Begin(winName.c_str(), &_open, flags))
    {
        std::chrono::duration<float> time = std::chrono::steady_clock::now() - _startTime;
        if(_preview)
            _preview->draw(time.count());
        
        drawControls();
        drawSubSystems();
    }
    ImGui::End();
}

void ParticlesWindow::drawControls()
{
    if(ImGui::Button("Restart"))
        _preview->getSystem()->reset();

    ImGui::Separator();

    if(ImGui::Button("OK"))
        _open = false;
    ImGui::SameLine();
    if(ImGui::Button("Restore"))
    {
        _particles->restore();
        _preview->setSystem(&_particles->getTemplate());
        _changed = false;
    }
    ImGui::SameLine();
    if(ImGui::Button("Export"))
    {
        ifd::FileDialog::Instance().Save(
            "particles_export",
            "Particles export",
            "Particles file (*.yaml){.yaml}"
        ); 
    }

    if(ifd::FileDialog::Instance().IsDone("particles_export"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string particlesFile(ifd::FileDialog::Instance().GetResult());
            _particles->save(particlesFile);
            _changed = false;
        }
        ifd::FileDialog::Instance().Close();
    }    
}

void ParticlesWindow::drawSubSystems()
{
    for(int i=0;i<_particles->getTemplate().getSubCount();++i)
    {
        Particles::SubSystemTemplate *sub = _particles->getTemplate().getSubSystem(i);
        if(ImGui::CollapsingHeader(sub->getName().c_str()))
            drawSubSystem(i);
    }
}

void ParticlesWindow::drawSubSystem(int i)
{
    Particles::SubSystemTemplate *sub = _particles->getTemplate().getSubSystem(i);
    Particles::SubSystem* subInstance = _preview->getSystem()->getSubSystem(i);

    char name[64];
    strcpy(name, sub->getName().c_str());
    if(ImGui::InputText("Name", name, 64));
        sub->setName(name);

    int rendererIdx = findIndex(sub->getRenderer(), RenderersNames, RenderersCount);
    int emitterIdx = findIndex(sub->getEmitter(), EmittersNames, EmittersCount);
    if(ImGui::Combo("Renderer", &rendererIdx, RenderersNames, IM_ARRAYSIZE(RenderersNames)))
    {
        sub->setRenderer(RenderersNames[rendererIdx]);
        subInstance->setRenderer(RenderersNames[rendererIdx]);
        _changed = true;
    }
    if(ImGui::Combo("Emitter", &emitterIdx, EmittersNames, IM_ARRAYSIZE(EmittersNames)))
    {
        sub->setEmitter(EmittersNames[emitterIdx]);
        subInstance->setEmitter(EmittersNames[emitterIdx]);
        _changed = true;
    }

    ImGui::Separator();
    drawMaterial(sub->getMaterial());

    ImGui::Separator();
    drawGlobalParams(sub, subInstance);

    ImGui::Separator();
    drawAllParticlesParams(sub);

    ImGui::Separator();
    if(ImGui::Button("New"))
    {
        std::string newName("new");
        while(subSystemExists(newName))
            newName += "new";
        auto newSub = new Particles::SubSystemTemplate(
            newName,
            "point",
            "point",
            new Particles::Material("Particles/Textures/smoke.tga", false),
            Particles::SubSystemParams(),
            Particles::SubSystemParams(),
            50,
            Core::Vector3::NullVector,
            10.f, 0.f
        );
        _particles->getTemplate().addSubSystem(newSub);
        _preview->getSystem()->addSubSystem(
            new Particles::SubSystem(newSub)
        );
    }
    ImGui::SameLine();
    if(ImGui::Button("Copy"))
    {
        std::string copyName(sub->getName() + "_copy");
        while(subSystemExists(copyName))
            copyName += "_copy";
        auto copySub = new Particles::SubSystemTemplate(*sub);
        copySub->setName(copyName);
        _particles->getTemplate().addSubSystem(copySub);
        _preview->getSystem()->addSubSystem(
            new Particles::SubSystem(copySub)
        );
    }
    ImGui::SameLine();
    if(ImGui::Button("Remove"))
    {
        _particles->getTemplate().remSubSystem(sub);
        _preview->getSystem()->remSubSystem(subInstance);
    }
}

void ParticlesWindow::drawMaterial(Particles::Material* mat)
{
    IVideoDevice* device = Renderer::getSingletonRef().getVideoDevice();
    BlendState blendState = mat->getBlendState();
    bool blendEnabled, newBlendEnabled;
    BlendMode srcBlend, destBlend;
    device->getBlendStateDesc(blendState, blendEnabled, srcBlend, destBlend);
    newBlendEnabled = blendEnabled;

    ImGui::Checkbox("Blend enabled", &newBlendEnabled);
    if(!newBlendEnabled)
        ImGui::BeginDisabled();

    bool blendChanged = newBlendEnabled != blendEnabled;
    if(ImGui::Combo("Source", (int*)&srcBlend, BlendNames, IM_ARRAYSIZE(BlendNames)))
        blendChanged = true;
    if(ImGui::Combo("Destination", (int*)&destBlend, BlendNames, IM_ARRAYSIZE(BlendNames)))
        blendChanged = true;

    if(blendChanged)
    {
        device->destroyBlendState(blendState);
        blendState = device->createBlendState(
            newBlendEnabled,
            srcBlend, destBlend
        );
        mat->setBlendState(blendState);
        _changed = true;
    }

    if(!newBlendEnabled)
        ImGui::EndDisabled();

    drawTexture(mat);
    drawTextureSet(mat);
}

void ParticlesWindow::drawTexture(Particles::Material* mat)
{
    ImGui::Text("Texture");
    ImGui::SameLine();
    std::string texName = mat->getTexture() ? mat->getTexture()->getName() : "< >";
    if(ImGui::Button(texName.c_str()))
    {
        ifd::FileDialog::Instance().Open(
            "particles_tex",
            "Select a texture",
            "Image file (*.*)",
            false
        );
    }
    if(ifd::FileDialog::Instance().IsDone("particles_tex"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string newTex(ifd::FileDialog::Instance().GetResult());
            newTex = getRelativePath(newTex);

            if(newTex != texName)
            {
                mat->setTexture(newTex);
                _changed = true;
            }
        }
        ifd::FileDialog::Instance().Close();
    }
}

void ParticlesWindow::drawTextureSet(Particles::Material* mat)
{
    ImGui::Text("Texture set");
    ImGui::SameLine();
    std::string texName = mat->getTextureSet() ? mat->getTextureSet()->getFolder() : "< >";
    if(ImGui::Button(texName.c_str()))
    {
        ifd::FileDialog::Instance().Open(
            "particles_texset",
            "Select a texture folder",
            "Image file (*.*)",
            false
        );
    }
    if(ifd::FileDialog::Instance().IsDone("particles_texset"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string newTex(ifd::FileDialog::Instance().GetResult());
            newTex = getPathOnly(getRelativePath(newTex));

            if(newTex != texName)
            {
                mat->setTextureSet(newTex);
                _changed = true;
            }
        }
        ifd::FileDialog::Instance().Close();
    }
}

void ParticlesWindow::drawGlobalParams(
    Particles::SubSystemTemplate* sub,
    Particles::SubSystem* subInstance
)
{
    int maxParticles = sub->getMaxParticles();
    if(ImGui::InputInt("Max particles", &maxParticles))
    {
        sub->setMaxParticles(maxParticles);
        subInstance->setMaxParticles(maxParticles);
        _changed = true;
    }

    float emissionRate = sub->getEmissionRate();
    if(ImGui::InputFloat("Emission rate", &emissionRate, 1.f, 10.f))
    {
        sub->setEmissionRate(emissionRate);
        _changed = true;
    }

    float lifeTime = sub->getLifeTime();
    if(ImGui::InputFloat("Lifetime", &lifeTime, 0.5f, 5.f))
    {
        sub->setLifeTime(lifeTime);
        _changed = true;
    }

    Core::Vector3 gravity(sub->getGravity());
    if(ImGui::InputFloat3("Gravity", (float*)&gravity))
    {
        sub->setGravity(gravity);
        _changed = true;
    }
}

void ParticlesWindow::drawAllParticlesParams(Particles::SubSystemTemplate* sub)
{
    Particles::SubSystemParams& pmin(sub->getMinParams());
    Particles::SubSystemParams& pmax(sub->getMaxParams());
    float space = ImGui::GetContentRegionAvail().x * .5f;

    _changed |= ImGui::InputFloat("Min life", &pmin.life, .1f, 1.f);
    ImGui::SameLine(space);
    _changed |= ImGui::InputFloat("Max life", &pmax.life, .1f, 1.f);

    _changed |= ImGui::InputFloat("Min mass", &pmin.mass, .1f, 1.f);
    ImGui::SameLine(space);
    _changed |= ImGui::InputFloat("Max mass", &pmax.mass, .1f, 1.f);

    _changed |= ImGui::InputFloat("Min friction", &pmin.friction, .1f, 1.f);
    ImGui::SameLine(space);
    _changed |= ImGui::InputFloat("Max friction", &pmax.friction, .1f, 1.f);

    _changed |= ImGui::ColorEdit4("Min start color", (float*)&pmin.startColor);
    ImGui::SameLine(space);
    _changed |= ImGui::ColorEdit4("Max start color", (float*)&pmax.startColor);

    _changed |= ImGui::ColorEdit4("Min end color", (float*)&pmin.endColor);
    ImGui::SameLine(space);
    _changed |= ImGui::ColorEdit4("Max end color", (float*)&pmax.endColor);

    _changed |= ImGui::InputFloat3("Min velocity", (float*)&pmin.velocity);
    ImGui::SameLine(space);
    _changed |= ImGui::InputFloat3("Max velocity", (float*)&pmax.velocity);

    _changed |= ImGui::InputFloat3("Min start size", (float*)&pmin.startSize);
    ImGui::SameLine(space);
    _changed |= ImGui::InputFloat3("Max start size", (float*)&pmax.startSize);

    _changed |= ImGui::InputFloat3("Min end size", (float*)&pmin.endSize);
    ImGui::SameLine(space);
    _changed |= ImGui::InputFloat3("Max end size", (float*)&pmax.endSize);
}

bool ParticlesWindow::subSystemExists(const std::string& name) const
{
    for(int i=0;i<_particles->getTemplate().getSubCount();++i)
        if(_particles->getTemplate().getSubSystem(i)->getName() == name)
            return true;
    return false;
}

}