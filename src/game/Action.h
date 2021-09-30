
#ifndef _ACTION_H_
#define _ACTION_H_

#include <string>

using namespace std;

class Action
{
protected:

	bool				_dead;
	float				_delayTime;
	string				_name;

public:

	Action(const char* name) : _name(name), _dead(false)				{}
	virtual ~Action()					{}

	const string&		getName() const				{ return _name; }
	bool				isDead() const				{ return _dead; }
	float&				getDelayTime()				{ return _delayTime; }

	virtual void		update(float time) = 0;
};

#endif