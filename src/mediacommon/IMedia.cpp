#include "IMedia.h"
#include "EventPump.h"
#include "IVideoDevice.h"
#include "ISoundDevice.h"

IMedia::IMedia() : _video(0), _audio(0)
{
    new EventPump;
}

IMedia::~IMedia()
{
    EventPump::deleteSingleton();
    delete _video;
    delete _audio;
}