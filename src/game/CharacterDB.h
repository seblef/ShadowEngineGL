
#pragma once

#include "Character.h"
#include "../core/TDataBase.h"

class CharacterDB : public Core::TDataBase<Character>
{
public:

	CharacterDB(const string& playersFile);

	void				unloadCharacters();
};
