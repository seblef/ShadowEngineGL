 
cmake_minimum_required(VERSION 3.10)

project(ShadowEngine)

find_package(OpenGL REQUIRED)

# Compile external dependencies
add_subdirectory (external)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
    -D_DEBUG
	-D_CRT_SECURE_NO_WARNINGS
)

set(PHYSX_LIBS
    ${CMAKE_SOURCE_DIR}/external/PhysX/bin/libPhysX_static_64.a
    ${CMAKE_SOURCE_DIR}/external/PhysX/bin/libPhysXPvdSDK_static_64.a
    ${CMAKE_SOURCE_DIR}/external/PhysX/bin/libPhysXCharacterKinematic_static_64.a
    ${CMAKE_SOURCE_DIR}/external/PhysX/bin/libPhysXVehicle_static_64.a
    ${CMAKE_SOURCE_DIR}/external/PhysX/bin/libPhysXExtensions_static_64.a
    ${CMAKE_SOURCE_DIR}/external/PhysX/bin/libPhysXCooking_static_64.a
    ${CMAKE_SOURCE_DIR}/external/PhysX/bin/libPhysXCommon_static_64.a
    ${CMAKE_SOURCE_DIR}/external/PhysX/bin/libPhysXFoundation_static_64.a
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
    ${PHYSX_LIBS}
	GLEW_220
    glfw
    -lopenal
    -lalut
    -lyaml-cpp
)


include_directories(
	external/glew-2.2.0/include/
    external/glfw-3.3.4/include/
    external/PhysX/include/
    external/PxShared/include/
    external/tclap/include
)


set(CORE_SOURCES
    src/loguru.cpp

    src/core/AnimPRSKeyVal.cpp
    src/core/AnimTracks.cpp
    src/core/BBox.cpp
    src/core/BSphere.cpp
    src/core/Camera.cpp
    src/core/CharUtil.cpp
    src/core/core.cpp
    src/core/FileSystemFactory.cpp
    src/core/IFileList.cpp
    src/core/IFileSystem.cpp
    src/core/Matrix.cpp
    src/core/MatrixHierarchy.cpp
    src/core/Plane.cpp
    src/core/Quaternion.cpp
    src/core/StdFile.cpp
    src/core/StdFileList.cpp
    src/core/StdFileSystem.cpp
    src/core/Vector.cpp
    src/core/ViewFrustum.cpp
)

set(AI_SOURCES
    src/ai/AIAgent.cpp
    src/ai/AIMachine.cpp
    src/ai/AISystem.cpp
    src/ai/AttackIdle.cpp
    src/ai/AttackMachine.cpp
    src/ai/AttackSearch.cpp
    src/ai/AttackTrack.cpp
    src/ai/FollowIdle.cpp
    src/ai/FollowMachine.cpp
    src/ai/FollowSearch.cpp
    src/ai/FollowTrack.cpp
    src/ai/GazeIdle.cpp
    src/ai/GazeMachine.cpp
    src/ai/GazeTrack.cpp
)

set(NAVIGATION_SOURCES
    src/navigation/AS_PathFinder.cpp
    src/navigation/NavMap.cpp
    src/navigation/NavPath.cpp
    src/navigation/NavRenderer.cpp
)

set(MEDIA_SOURCES
    src/mediacommon/EventPump.cpp
    src/mediacommon/MediaCommon.cpp
    src/mediacommon/IMedia.cpp
)

set(OPENGL_SOURCES
    src/glmedia/SOIL/image_DXT.c
    src/glmedia/SOIL/image_helper.c
    src/glmedia/SOIL/SOIL.c
    src/glmedia/SOIL/stb_image_aug.c
    src/glmedia/ALSound.cpp
    src/glmedia/ALSoundDevice.cpp
    src/glmedia/ALSoundListener.cpp
    src/glmedia/ALSoundSource.cpp
    src/glmedia/ALSoundVirtualSource.cpp
    src/glmedia/GLConstantBuffer.cpp
    src/glmedia/GLDepthTexture.cpp
    src/glmedia/GLFrameBuffer.cpp
    src/glmedia/GLGeometryBuffer.cpp
    src/glmedia/GLIndexBuffer.cpp
    src/glmedia/GLShader.cpp
    src/glmedia/GLShadowMap.cpp
    src/glmedia/GLTexture.cpp
    src/glmedia/GLVertexBuffer.cpp
    src/glmedia/GLVideoDevice.cpp
    src/glmedia/Media.cpp
)

set(SOUND_SOURCES
    src/sound/SoundSystem.cpp
    src/sound/SoundVirtualSource.cpp
)

set(RENDERER_SOURCES
    src/renderer/Actor.cpp
    src/renderer/ActorAnimInstance.cpp
    src/renderer/ActorInstance.cpp
    src/renderer/ActorRenderer.cpp
    src/renderer/ActorSkinnedRenderer.cpp
    src/renderer/BufferView.cpp
    src/renderer/GBuffer.cpp
    src/renderer/GeometryData.cpp
    src/renderer/GroundData.cpp
    src/renderer/GroundRenderer.cpp
    src/renderer/HDAO.cpp
    src/renderer/HDR.cpp
    src/renderer/IUpdatable.cpp
    src/renderer/Light.cpp
    src/renderer/LightAreaRenderer.cpp
    src/renderer/LightGlobal.cpp
    src/renderer/LightOmniRenderer.cpp
    src/renderer/LightRenderer.cpp
    src/renderer/LightSpotRenderer.cpp
    src/renderer/LightSystem.cpp
    src/renderer/Material.cpp
    src/renderer/MaterialAnimationFactory.cpp
    src/renderer/MaterialStdBuffer.cpp
    src/renderer/MaterialSystem.cpp
    src/renderer/MaterialAnimation.cpp
    src/renderer/MeshInstance.cpp
    src/renderer/MeshRenderer.cpp
    src/renderer/Particles.cpp
    src/renderer/QuadTreeNode.cpp
    src/renderer/Renderable.cpp
    src/renderer/Renderer.cpp
    src/renderer/ShadowMapsBase.cpp
    src/renderer/ShadowMapBuffer.cpp
    src/renderer/ShadowSystem.cpp
    src/renderer/UpdateSystem.cpp
    src/renderer/VisibilityAll.cpp
    src/renderer/VisibilityFactory.cpp
    src/renderer/VisibilityQuad.cpp
)

set(PARTICLES_SOURCES
    src/particles/ParticleEmitter.cpp
    src/particles/ParticleEmitterFactory.cpp
    src/particles/ParticleEmitterTemplate.cpp
    src/particles/ParticleEmitterTemplateFactory.cpp
    src/particles/ParticleEngine.cpp
    src/particles/ParticleExplosionEmitter.cpp
    src/particles/ParticleLinearEmitter.cpp
    src/particles/ParticleMaterial.cpp
    src/particles/ParticleRenderer.cpp
    src/particles/ParticleSystem.cpp
    src/particles/ParticleSystemTemplate.cpp
    src/particles/ParticleTextureSet.cpp
    src/particles/PR_Point.cpp
    src/particles/PR_Quad.cpp
)

set(RENDERER2D_SOURCES
    src/renderer2d/R2D_Font.cpp
    src/renderer2d/R2D_FontLoader.cpp
    src/renderer2d/R2D_FrameLoader.cpp
    src/renderer2d/R2D_Image.cpp
    src/renderer2d/R2D_ImageRenderer.cpp
    src/renderer2d/R2D_LineRenderer.cpp
    src/renderer2d/R2D_RectRenderer.cpp
    src/renderer2d/R2D_Renderer.cpp
    src/renderer2d/R2D_StdFontLoader.cpp
    src/renderer2d/R2D_StdFrameLoader.cpp
    src/renderer2d/R2D_TextRenderer.cpp
    src/renderer2d/Renderer2D.cpp
)

set(PHYSIC_SOURCES
    src/physic/Physic.cpp
    src/physic/PhysicActorController.cpp
    src/physic/PhysicAmmo.cpp
    src/physic/PhysicDynamic.cpp
    src/physic/PhysicGeometry.cpp
    src/physic/PhysicGround.cpp
    src/physic/PhysicKinematicScene.cpp
    src/physic/PhysicObject.cpp
    src/physic/PhysicStaticScene.cpp
    src/physic/PhysicTrigger.cpp
)

set(GAME_SOURCES
    src/game/ActionCreateRandomCharacter.cpp
    src/game/ActionServer.cpp
    src/game/ActorACTLoader.cpp
    src/game/ActorAnimLoader.cpp
    src/game/ActorLoader.cpp
    src/game/AddRemObject.cpp
    src/game/BBoxToRect.cpp
    src/game/Building.cpp
    src/game/BuildingTemplate.cpp
    src/game/Character.cpp
    src/game/CharacterDB.cpp
    src/game/Debris.cpp
    src/game/EffectDB.cpp
    src/game/EffectFactory.cpp
    src/game/EntityDB.cpp
    src/game/EntityDynamic.cpp
    src/game/EntityExplosive.cpp
    src/game/EntityFactory.cpp
    src/game/EntityTemplate.cpp
    src/game/EntityTrigger.cpp
    src/game/EntityUpdater.cpp
    src/game/Explosion.cpp
    src/game/ExplosionTemplate.cpp
    src/game/FieldHit.cpp
    src/game/Flash.cpp
    src/game/FPSCounter.cpp
    src/game/GameActorUpdater.cpp
    src/game/GameCamera.cpp
    src/game/GameCharacter.cpp
    src/game/GameEntity.cpp
    src/game/GameEntityDynamic.cpp
    src/game/GameEntityExplosive.cpp
    src/game/GameEntityTrigger.cpp
    src/game/GameLight.cpp
    src/game/GameMaterial.cpp
    src/game/GameMesh.cpp
    src/game/GameObjectSet.cpp
    src/game/GameParticles.cpp
    src/game/GamePlayer.cpp
    src/game/GameSound.cpp
    src/game/GameSystem.cpp
    src/game/Geo3DSLoader.cpp
    src/game/GeoGEOLoader.cpp
    src/game/Geometry.cpp
    src/game/GeometryLoader.cpp
    src/game/Ground.cpp
    src/game/GroundBinLoader.cpp
    src/game/HUD.cpp
    src/game/HUDActorView.cpp
    src/game/HUDCross.cpp
    src/game/Map.cpp
    src/game/MapLoaderFactory.cpp
    src/game/MapTextLoader.cpp
    src/game/Navigator.cpp
    src/game/PhysicEventProcessor.cpp
    src/game/Rocket.cpp
    src/game/SystemValues.cpp
    src/game/TemplateMesh.cpp
    src/game/TemplateSound.cpp
    src/game/TextParser.cpp
    src/game/Trace.cpp
    src/game/TraceTemplate.cpp
    src/game/Trigger.cpp
    src/game/TriggerWeapon.cpp
    src/game/Weapon.cpp
    src/game/WeaponDB.cpp
    src/game/WeaponFactory.cpp
    src/game/WeaponGun.cpp
    src/game/WeaponGunInstance.cpp
    src/game/WeaponInstance.cpp
    src/game/WeaponRocket.cpp
    src/game/WeaponRocketInstance.cpp
)

set(EDITOR_SOURCES
    src/editor/imgui/imgui.cpp
    src/editor/imgui/imgui_demo.cpp
    src/editor/imgui/imgui_draw.cpp
    src/editor/imgui/imgui_tables.cpp
    src/editor/imgui/imgui_widgets.cpp
    src/editor/imgui/backends/imgui_impl_glfw.cpp
    src/editor/imgui/backends/imgui_impl_opengl3.cpp
    src/editor/filedialog/ImFileDialog.cpp
    src/editor/EdCamera.cpp
    src/editor/EdMaterial.cpp
    src/editor/EditorSystem.cpp
    src/editor/Helpers.cpp
    src/editor/IWindow.cpp
    src/editor/MainMenu.cpp
    src/editor/MaterialWindow.cpp
    src/editor/NavigationPanel.cpp
    src/editor/PreviewCamera.cpp
    src/editor/PreviewMesh.cpp
    src/editor/PreviewResources.cpp
    src/editor/Resources.cpp
)

set(SHADOW_SOURCES
    src/shadow/main.cpp
    src/shadow/EditorApp.cpp
    src/shadow/GameApp.cpp
    src/shadow/NavTest.cpp
    src/shadow/ParticleApp.cpp
    src/shadow/TestGLApp.cpp
    src/shadow/TestSoundApp.cpp
)

add_executable(ShadowEngine
    ${CORE_SOURCES}
    ${AI_SOURCES}
    ${NAVIGATION_SOURCES}
    ${MEDIA_SOURCES}
    ${OPENGL_SOURCES}
    ${SOUND_SOURCES}
    ${RENDERER_SOURCES}
    ${PARTICLES_SOURCES}
    ${RENDERER2D_SOURCES}
    ${PHYSIC_SOURCES}
    ${GAME_SOURCES}
    ${EDITOR_SOURCES}
    ${SHADOW_SOURCES}
)


target_link_libraries(ShadowEngine
 ${ALL_LIBS}
)

