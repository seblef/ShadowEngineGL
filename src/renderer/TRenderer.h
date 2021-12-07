#pragma once

#include "../mediacommon/IShader.h"
#include <algorithm>
#include "Material.h"

namespace Core
{
    class ViewFrustum;
}

template<typename T>
class TRenderer
{
protected:

	IVideoDevice*				_device;

	IShader*					_gbufferShader;
	IShader*					_addShader;
	IShader*					_shadowShader;
	IShader*					_shadowAlphaShader;

	typedef vector<T*>			InstanceVector;
	InstanceVector				_instances;
	InstanceVector				_shadowQueue;
	InstanceVector				_shadowAlphaQueue;

	static bool					materialSort(const T* i1, const T* i2)
	{
		return i1->getModel()->getMaterial() < i2->getModel()->getMaterial();
	}

	static bool					modelSort(const T* i1, const T* i2)
	{
		return i1->getModel() < i2->getModel();
	}

public:

	TRenderer(IVideoDevice* device, const string& gbufferShader, const string& addShader,
		const string& shadowShader, const string& shadowAlphaShader)
		: _device(device)
	{
		_gbufferShader=gbufferShader.empty() ? 0 : device->createShader(gbufferShader);
		_addShader=addShader.empty() ? 0 : device->createShader(addShader);
		_shadowShader=shadowShader.empty() ? 0 : device->createShader(shadowShader);
		_shadowAlphaShader = shadowAlphaShader.empty() ? 0 : device->createShader(shadowAlphaShader);
	}
	virtual ~TRenderer()
	{
		if(_gbufferShader)		_gbufferShader->remRef();
		if(_addShader)			_addShader->remRef();
		if(_shadowShader)		_shadowShader->remRef();
		if (_shadowAlphaShader)	_shadowAlphaShader->remRef();
	}

	void						addInstance(T* i)					{ _instances.push_back(i); }

	void						prepareShadows()
	{
        typename InstanceVector::iterator i(_instances.begin());
		for (; i != _instances.end(); ++i)
		{
			Material *m = (*i)->getModel()->getMaterial();
			if (!m->getFlag(MF_CASTNOSHADOWS))
			{
				if (m->getFlag(MF_ALPHASHADOWS))
					_shadowAlphaQueue.push_back(*i);
				else
					_shadowQueue.push_back(*i);
			}
		}
	}

	void						prepareRender()
	{
		sort(_instances.begin(),_instances.end(),materialSort);
        typename InstanceVector::iterator ibegin,iend,i;
		i=ibegin=_instances.begin();
		while(i!=_instances.end())
		{
			Material* m=(*ibegin)->getModel()->getMaterial();
			while(i!=_instances.end() && (*i)->getModel()->getMaterial()==m)
				++i;

			sort(ibegin,i,modelSort);
			ibegin=i;
		}

		prepareShadows();
	}

	void						endRender()
	{
		_instances.clear();
		_shadowQueue.clear();
		_shadowAlphaQueue.clear();
	}

	virtual void				render(bool addPass)=0;
	virtual void				renderShadows(const ViewFrustum& vf)=0;

};
