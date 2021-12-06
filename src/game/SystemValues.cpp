
#include "SystemValues.h"
#include "../core/YAMLCore.h"


void SystemValues::parseConfig(const YAML::Node& node)
{
	_showFPS = node["show_fps"].as<bool>(_showFPS);
	_hudFont = node["hud_font"].as<string>(_hudFont);
	_crossTexture = node["cross_texture"].as<string>(_crossTexture);
	_crossSize = node["cross_size"].as<float>(_crossSize);
	_cameraBaseDistance = node["camera_base_distance"].as<float>(_cameraBaseDistance);
	_gravity = node["gravity"].as<Vector3>(_gravity);
	_aiDamageFactor = node["ai_damage_factor"].as<float>(_aiDamageFactor);
	_playerDamageFactor = node["player_damage_factor"].as<float>(_playerDamageFactor);
	_damageToForce = node["damage_to_force"].as<float>(_damageToForce);
	_deadActorStayOnMapTime = node["dead_actors_stay_on_map"].as<float>(_deadActorStayOnMapTime);

    YAML::Node deb(node["base_debris"]);
	if(deb)
	{
		_baseDebrisCount = deb["count"].as<int>(_baseDebrisCount);
		_baseDebrisMinVelocity = deb["min_velocity"].as<float>(_baseDebrisMinVelocity);
		_baseDebrisMaxVelocity = deb["max_velocity"].as<float>(_baseDebrisMaxVelocity);
		_baseDebrisAngle = deb["angle"].as<float>(_baseDebrisAngle);
		_baseDebrisAngleSpeed = deb["angle"].as<float>(_baseDebrisAngleSpeed);
		_baseDebrisSize = deb["size"].as<float>(_baseDebrisSize);
	}
}
