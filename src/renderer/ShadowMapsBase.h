#pragma once

class IShadowMap;
class IVideoDevice;

class ShadowMapsBase
{
public:

	enum MapSize
	{
		MP_64=0,
		MP_128,
		MP_256,
		MP_512,
		MP_1024,

		MP_COUNT
	};


protected:

	static const int		_mapSizes[MP_COUNT];
	static const int		_mapCounts[MP_COUNT];

	int						_leftMaps[MP_COUNT];
	IShadowMap**			_maps[MP_COUNT];

	MapSize					getBestSize(int size) const;

public:

	ShadowMapsBase(IVideoDevice *device);
	~ShadowMapsBase();

	void					clear();

	IShadowMap*				getShadowMap(int size);
	IShadowMap*				getShadowMap(MapSize size, int n) const				{ return _maps[size][n]; }
};
