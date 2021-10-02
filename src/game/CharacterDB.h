
#pragma once

#include "Character.h"

class CharacterDB : public TDataBase<Character>
{
public:

	CharacterDB(const string& playersFile);

	void				unloadCharacters();
};
