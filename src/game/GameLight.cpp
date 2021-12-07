#include "GameLight.h"
#include "../renderer/Renderer.h"
#include "../renderer/LightArea.h"
#include "../renderer/LightOmni.h"
#include "../renderer/LightSpot.h"


GameLight::GameLight(const LightCreate_t& c, Light::LightType type) : GameObject(LIGHT,c._world,BBox3(-c._range,-c._range,-c._range,c._range,c._range,c._range),OF_NOCOLLISION)
{
    switch (type)
    {
    case Light::LT_OMNI:
        _rLight=new LightOmni(c);
        break;
    case Light::LT_SPOT:
        _rLight=new LightSpot(c);
        break;
    case Light::LT_AREA:
        _rLight=new LightArea(c);
        break;

    default:
        _rLight=new LightOmni(c);
    }
}

GameLight::~GameLight()
{
    delete _rLight;
}

void GameLight::onAddToScene()
{
    Renderer::getSingletonRef().addRenderable(_rLight);
}

void GameLight::onRemFromScene()
{
    Renderer::getSingletonRef().remRenderable(_rLight);
}

void GameLight::onUpdateMatrix()
{
    _rLight->setWorldMatrix(_world);
}
