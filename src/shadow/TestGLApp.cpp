
#include "TestGLApp.h"
#include "../GameLib.h"
#include "../SoundLib.h"
#include "../GLMedia.h"
#include "../R2D.h"
#include "../StdRenderer.h"
#include "../game/GeometryLoader.h"
#include "../game/ActorLoader.h"
#include "../game/ActorAnimLoader.h"
#include "../renderer/ShadowSystem.h"
#include "../loguru.hpp"
#include <time.h>
#include <sstream>

//#define HDAO_TUNING

#ifdef HDAO_TUNING
#include "../Renderer/HDAO.h"
#endif


void TestGLApp::createPlane(IVideoDevice *d)
{
    Vx3D v[6]={
        Vx3D(Vector3(-5.0f,0.0f,-5.0f),Vector3::YAxisVector,Vector2::NullVector),
        Vx3D(Vector3(5.0f,0.0f,5.0f),Vector3::YAxisVector,Vector2::UnitVector),
        Vx3D(Vector3(5.0f,0.0f,-5.0f),Vector3::YAxisVector,Vector2::XAxisVector),
        Vx3D(Vector3(-5.0f,0.0f,-5.0f),Vector3::YAxisVector,Vector2::NullVector),
        Vx3D(Vector3(-5.0f,0.0f,5.0f),Vector3::YAxisVector,Vector2::YAxisVector),
        Vx3D(Vector3(5.0f,0.0f,5.0f),Vector3::YAxisVector,Vector2::UnitVector)
    };

    unsigned short ind[6]={
      0,1,2,3,4,5
    };

    Geometry *geo=new Geometry(6,2);
    memcpy(geo->getVertices(),v,sizeof(Vx3D) * 6);
    memcpy(geo->getTriangles(),ind,sizeof(unsigned short) * 6);
    geo->setBBox(BBox3(-5.0f,-0.5f,-5.0f,5.0f,0.5f,5.0f));
    geo->buildRGeometry();
    geo->computeBinormalAndTangent();
    GameMaterial *mat=GameMaterial::loadMaterial("Materials/Floor");

    Mesh *tm=new Mesh(geo->getRGeometry(),mat->getRMaterial());
    MeshInstance *m=new MeshInstance(tm,Matrix4::Identity,true);

    Renderer::getSingletonRef().addRenderable(m);
}

Light*     g_ShadowLight=0;

void TestGLApp::run(const string& dataFolder)
{
    initShadowCore();

    FileSystemFactory::createFileSystem("std", dataFolder);
    Config cfg("shadow.ini");

    string configFile("Game/config.txt");
    cfg.getVar("configFile", configFile);
    cfg.parseFile(configFile);

    int w = 1024;
    int h = 768;
    bool windowed = true;
    bool multithread = false;

/*	cfg.getVar("resWidth", w);
    cfg.getVar("resHeight", h);
    cfg.getVar("windowed", windowed);
    cfg.getVar("multithread", multithread);*/

    IMedia *media=createGLMedia(w,h,windowed,multithread,SINPUT_MOUSE | SINPUT_KEYBOARD);
    new SoundSystem(media->getAudio());
    new Renderer(media->getVideo(),cfg);
    new Renderer2D(media->getVideo());

    Vector2 camRot(Vector2::NullVector);

    Vector3 dir(0.0f,-1.0f,-1.0f);
    dir.normalize();

    LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(Color::White*0.5f,dir);
//    LightSystem::getSingletonRef().getGlobalLight().disableGlobalLight();
//    LightSystem::getSingletonRef().getGlobalLight().setAmbientLight(Color::White);

    createPlane(media->getVideo());

    LightCreate_t lc;
    lc._world.createTranslate(-1.0f,2.0f,0.0f);
    lc._color=Color::Red;
    lc._color.a=2.0f;
    lc._range=5.0f;
    lc._castShadows=true;
    lc._direction=Vector3(1.0f,-1.0f,0.0f);
    lc._direction.normalize();
    lc._nearAngle=45.0f;
    lc._farAngle=90.0f;
    lc._nearZ=0.1f;
    lc._areaWidth=2.0f;
    lc._areaHeight=1.5f;
    lc._shadowMapSize=256;
    lc._shadowMapFilter=4;
    Light::computeMatrix(lc._direction,lc._world);
    LightSpot *l=new LightSpot(lc);
    g_ShadowLight=l;


    Geometry *geo=GeometryLoader::loadGeometry("Geometry/Barrel.geo");
    geo->buildRGeometry();
    GameMaterial *mat=GameMaterial::loadMaterial("Materials/Barrel");

    Mesh *tm=new Mesh(geo->getRGeometry(),mat->getRMaterial());
    MeshInstance* mesh=new MeshInstance(tm,Matrix4::Identity,true);
    Matrix4 mw;
    mw.createTranslate(-1.0f,0.0f,0.0f);
    MeshInstance* mesh2=new MeshInstance(tm,mw,true);

    mw.createTranslate(-0.5f,0.0f,0.5f);
    MeshInstance* mesh3=new MeshInstance(tm,mw,true);

    GameMaterial *actorMat=GameMaterial::loadMaterial("Materials/Synthoid");
    Actor *actor=ActorLoader::loadActor("Synthoid.act",actorMat->getRMaterial(),media->getVideo());
    AnimPRSMultiTrack* actorAnim=ActorAnimLoader::loadActorAnimation("Actors/Synthoid/walk.aan");
    actor->addAnimation("walk",actorAnim);
    Matrix4 am;
    am.createTranslate(2.0f,-actor->getBBox().getMin().y,2.0f);
    ActorInstance *actorInstance=new ActorInstance(actor,Matrix4::Identity,true);

    Renderer::getSingletonRef().addRenderable(mesh);
    Renderer::getSingletonRef().addRenderable(mesh2);
    Renderer::getSingletonRef().addRenderable(mesh3);
    Renderer::getSingletonRef().addRenderable(l);
    Renderer::getSingletonRef().addRenderable(actorInstance);

    actorInstance->playAnimation("walk",true);
    actorInstance->wakeUp();

    Camera cam(Vector3(0.0f,1.0f,-1.0f),Vector3::NullVector,0.1f,100.0f,Vector2(512.0f,384.0f),90.0f);
    cam.setUpVector(Vector3::YAxisVector);
//    cam.setCameraType(CT_ORTHO);
    cam.center(BBox3(-3.0f,-0.5f,-3.0f,3.0f,0.5f,3.0f));
   cam.center(actorInstance->getWorldBBox());
   float camDist=(cam.getTarget() - cam.getPosition()).getLength();
//    cam.setPosition(cam.getPosition() + Vector3::XAxisVector*2.0f);
//    cam.setTarget(cam.getTarget() + Vector3::XAxisVector*2.0f);

   clock_t start=clock();

    bool end=false;
    while (!end)
    {
        LOG_S(INFO) << "*-------------------------- Game system update ---------------------------- *";
        static clock_t lastUpdate=clock();
        clock_t now=clock();

        float time=((float)(now-lastUpdate)) / ((float)CLOCKS_PER_SEC);
        LOG_S(INFO) << "Frame time: " << time;

        if (time > 0.5f)
            time = 0.5f;

        media->update();

        media->getVideo()->newFrame();
        media->getVideo()->resetRenderTargets();

//        ShadowSystem::getSingletonRef().getGlobalLightCamera(cam);
        LOG_S(INFO) << "------- Updating renderer...";
        actorInstance->setWorldMatrix(am);
        Renderer::getSingletonRef().update(time,&cam);

        LOG_S(INFO) << "------- Updating 2D renderer...";
//        Renderer2D::getSingletonRef().render();

        LOG_S(INFO) << "------- Processing user events...";
        bool end = false;
        Event e;
        while (EventPump::getSingletonRef().getEvent(e))
        {
            if (e._type == ET_CLOSE || (e._type == ET_KEYDOWN && e._param1 == DIK_ESCAPE))
                end = true;
            else if(e._type==ET_MOUSEMOVE)
            {
                camRot.x+=(float)e._param2;
                camRot.y+=(float)e._param1;
            }
        }

        Core::Quaternion rot_y(Core::Vector3::YAxisVector,camRot.y*M_PI / 180.0f);
        Core::Vector3 z(Core::Vector3::ZAxisVector);
        Core::Vector3 x(Core::Vector3::XAxisVector);

        x=x*rot_y;
        z=z*rot_y;

        Core::Quaternion rot_x(x,camRot.x*M_PI/180.0f);
        z=z*rot_x;

        cam.setPosition(cam.getTarget()-(z*camDist));


        lastUpdate=now;
    }

    clock_t tend=clock_t();

    LOG_S(INFO) << "FPS: " << ((float)CLOCKS_PER_SEC) / ((float)(tend-start));

    Renderer2D::deleteSingleton();
    Renderer::deleteSingleton();
    SoundSystem::deleteSingleton();
    delete media;
}
