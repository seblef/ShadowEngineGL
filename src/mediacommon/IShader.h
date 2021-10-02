#pragma once

#include <string>

using namespace std;

class IShader
{
protected:

	bool				_good;
	string				_name;

	int					_refCount;

public:

	IShader(const string& name) : _good(false), _name(name), _refCount(1) {}
	virtual ~IShader() {}

	bool				isGood() const				{ return _good; }
	const string&		getName() const				{ return _name; }

	virtual void		set()=0;

	virtual int			addRef() = 0;
	virtual int			remRef() = 0;
	int					getRefCount() const			{ return _refCount; }
};
