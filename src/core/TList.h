
#pragma once

#include <list>

using namespace std;

namespace Core
{

template<typename T>
class TList
{
protected:

	typedef list<T*>	TL;

	bool				_autoDelete;
	TL					_data;

public:

	TList(bool autoDelete=true) : _autoDelete(autoDelete)				{}
	~TList()															{ clear(); }

	void				clear()
	{
		if(_autoDelete)
		{
            for(const auto& i : _data)
                delete i;
		}

		_data.clear();
	}

	list<T*>&			getList()										{ return _data; }
	const list<T*>&		getList() const									{ return _data; }
};

};
