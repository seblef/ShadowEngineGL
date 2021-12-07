
#include "ShadowMapsBase.h"
#include "../mediacommon/IShadowMap.h"
#include "../mediacommon/IVideoDevice.h"


const int ShadowMapsBase::_mapSizes[ShadowMapsBase::MP_COUNT]={
	64,128,256,512,1024
};

const int ShadowMapsBase::_mapCounts[ShadowMapsBase::MP_COUNT]={
	16,16,16,8,4
};

ShadowMapsBase::ShadowMapsBase(IVideoDevice* device)
{
	for(int i=0;i<MP_COUNT;++i)
	{
		_maps[i]=new IShadowMap*[_mapCounts[i]];
		for(int j=0;j<_mapCounts[i];++j)
			_maps[i][j]=device->createShadowMap(_mapSizes[i],_mapSizes[i]);
	}

	clear();
}

ShadowMapsBase::~ShadowMapsBase()
{
	for(int i=0;i<MP_COUNT;++i)
	{
		for(int j=0;j<_mapCounts[i];++j)
			delete _maps[i][j];

		delete[] _maps[i];
	}
}

IShadowMap *ShadowMapsBase::getShadowMap(int size)
{
	MapSize ms=getBestSize(size);
	IShadowMap* m=0;
	if(_leftMaps[ms])
	{
		--_leftMaps[ms];
		m=_maps[ms][_leftMaps[ms]];
	}
	return m;
}

ShadowMapsBase::MapSize ShadowMapsBase::getBestSize(int size) const
{
    MapSize ms=MP_1024;
    for(int i=0;i<MP_COUNT;++i)
        if(size <= _mapSizes[i])
        {
            ms=(MapSize)i;
            break;
        }
    return ms;
}

void ShadowMapsBase::clear()
{
    for(int i=0;i<MP_COUNT;++i)
        _leftMaps[i]=_mapCounts[i];
}