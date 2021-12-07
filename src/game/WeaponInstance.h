
#pragma once


class GameCharacter;
class Weapon;
class MeshInstance;
class Hit;

namespace Core
{
    class Vector3;
}

class WeaponInstance
{
protected:

	Weapon*					_template;
	MeshInstance*			_rMeshInstance;
	GameCharacter*			_character;

	double					_lastFire;

	void					hit(const Hit& hit) const;
	void					fire(const Core::Vector3& origin) const;

public:

	WeaponInstance(Weapon* w, GameCharacter *c);
	virtual ~WeaponInstance();

	Weapon*					getTemplate() const		{ return _template; }
	MeshInstance*			getMeshInstance() const	{ return _rMeshInstance; }

	virtual void			fire()=0;

	bool					canFire() const;
};
