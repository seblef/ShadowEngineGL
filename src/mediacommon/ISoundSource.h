#pragma once


class ISoundVirtualSource;
class ISound;

class ISoundSource
{
public:

	ISoundSource()				{}
	virtual ~ISoundSource()		{}

	virtual void			play(ISoundVirtualSource* virtSrc, ISound* buffer, bool loop = false)=0;
	virtual void			stop()=0;

	virtual bool			isPlaying()=0;
};
