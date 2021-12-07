
#pragma once

#include "../core/TSingleton.h"
#include "../renderer2d/R2D_Layer.h"
#include "../renderer2d/R2D_Frame.h"

class HUDActor;
class HUDActorView;
class HUDCross;
class GameCamera;
class FPSCounter;
class R2D_Font;


class HUD : public Core::TSingleton<HUD>
{
protected:

	FPSCounter*				_fpsCounter;
	HUDCross*				_cross;

	R2D_Frame				*_frame;
	R2D_Font				*_hudFont;
	R2D_Layer				*_hudLayer;

	typedef set<HUDActor*>	ActorSet;
	ActorSet				_actors;
	HUDActorView*			_actorView;

public:

	HUD();
	~HUD();

	HUDActor*				createHUDActor(float maxLife, float actorHeight) const;
	void					registerActor(HUDActor* hudActor)	{ _actors.insert(hudActor); }
	void					unregisterActor(HUDActor* hudActor)	{ _actors.erase(hudActor); }

	void					updateHUD(GameCamera& gc);
};
