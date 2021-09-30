
#ifndef _GLMEDIA_H_
#define _GLMEDIA_H_

#include "MediaCommon.h"

IMedia*			createGLMedia(int width, int height, bool windowed, bool multithread, unsigned int input_flags);

#endif
