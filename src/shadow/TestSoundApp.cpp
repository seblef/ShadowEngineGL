
#include "TestSoundApp.h"
#include "../GLMedia.h"
#include "../SoundLib.h"
#include <time.h>
#include <sstream>



void TestSoundApp::run(const string& dataFolder)
{
	initShadowCore();

    FileSystemFactory::createFileSystem("std", dataFolder);

    IMedia *media = createGLMedia(800, 600, true, false, SINPUT_MOUSE | SINPUT_KEYBOARD);

	ISound *snd = media->getAudio()->loadSound("Sounds/Test.wav");
	ISoundVirtualSource* virt_src = media->getAudio()->createVirtualSource(
		Vector3::ZAxisVector,
		Vector3::NullVector,
		-Vector3::ZAxisVector,
		5.f, 10.f, 1.f, 0.f, 180.f
	);
	media->getAudio()->getSource(0)->play(virt_src, snd, true);

	clock_t lastTime = clock();
	float total_time = 0;

	bool end = false;
	while (!end)
	{
		clock_t now = clock();
		float elapsed = ((float)(now - lastTime)) / ((float)CLOCKS_PER_SEC);
		lastTime = now;

		total_time += elapsed;
		if (total_time > 2.0f)
		{
			total_time -= 2.0f;
		}

		media->update();

		media->getVideo()->resetRenderTargets();
		media->getVideo()->clearRenderTargets(Color(0.1f, 0.1f, 0.1f, 1.0f));
		media->getVideo()->newFrame();

		Event e;
		while (EventPump::getSingletonRef().getEvent(e))
		{
			if (e._type == ET_CLOSE || (e._type == ET_KEYDOWN && e._param1 == DIK_ESCAPE))
				end = true;
		}

	}
}
