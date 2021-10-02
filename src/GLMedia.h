#pragma once

#include "MediaCommon.h"

IMedia*			createGLMedia(int width, int height, bool windowed, bool multithread, unsigned int input_flags);
