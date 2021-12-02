
#include "CharacterDB.h"
#include "../loguru.hpp"
#include <yaml-cpp/yaml.h>


CharacterDB::CharacterDB(const string& playersFile)
{
	YAML::Node root;
	try
	{
		root = YAML::LoadFile(playersFile);
	}
	catch(const char *msg)
	{
		LOG_S(ERROR) << "Failed parsing " << playersFile << " character file: " << msg << endl;
		return;
	}

	assert(root.IsMap());

	for(YAML::const_iterator it=root.begin(); it!=root.end(); ++it)
	{
		const string& name(it->first.as<std::string>());
		YAML::Node props = it->second;

		Character::AnimMap anims;
		if(props["animations"])
		{
			for(YAML::const_iterator anim=props["animations"].begin(); anim!=props["animations"].end(); ++anim)
				anims[anim->first.as<string>()] = anim->second.as<string>();
		}

		Character *p = new Character(
			name,
			props["actor"].as<string>(),
			props["material"].as<string>(),
			props["forward_speed"].as<float>(1.f),
			props["backward_speed"].as<float>(.8f),
			props["lateral_speed"].as<float>(.5f),
			props["rotation_speed"].as<float>(60.f),
			props["weapon"].as<string>(""),
			props["ai_machine"].as<string>("Default"),
			props["ai_state"].as<string>(""),
			props["fov"].as<float>(90.f),
			props["visibility_distance"].as<float>(30.f),
			props["life"].as<float>(100.f),
			anims
		);

		registerData(name, p);
	}
}

void CharacterDB::unloadCharacters()
{
	for(auto const& character : _data)
		character.second->unload();
}
