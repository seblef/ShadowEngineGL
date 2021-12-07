#pragma once


#define SINPUT_NONE				0
#define SINPUT_MOUSE			1
#define SINPUT_KEYBOARD			2


class IVideoDevice;
class ISoundDevice;

class IMedia
{
protected:

	IVideoDevice*			_video;
	ISoundDevice*			_audio;

public:

	IMedia();
	virtual ~IMedia();

	IVideoDevice*			getVideo() const				{ return _video; }
	ISoundDevice*			getAudio() const				{ return _audio; }

	virtual void			update()=0;
};
