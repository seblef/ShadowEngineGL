
#pragma once

#include <string>
#include <map>

using namespace std;

namespace Core
{

template<typename T>
class TDataBase
{
protected:

	bool						_autoDelete;
	typedef map<string,T*>	TMap;
	TMap						_data;

public:

	TDataBase(bool autoDelete=true) : _autoDelete(autoDelete)			{}
	~TDataBase()														{ clear(); }

	void						clear()
	{
		if(_autoDelete)
		{
			for(auto const& i : _data)
				delete i.second;
		}

		_data.clear();
	}

	T*							getData(const string& name)
	{
        typename TMap::iterator d(_data.find(name));
		return d==_data.end() ? 0 : d->second;
	}

	void						registerData(const string& name, T* d)
	{
		T* old=getData(name);
		if(old)			delete old;

		_data[name]=d;
	}
	void						unregisterData(const string& name)
	{
        typename TMap::iterator d(_data.find(name));
		if (d != _data.end())
			_data.erase(_data.find(name));
	}

	const map<string,T*>&		getData() const								{ return _data; }
};

};
